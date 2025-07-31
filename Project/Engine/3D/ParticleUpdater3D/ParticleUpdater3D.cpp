#include "ParticleUpdater3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/ComputePipelineManager/ComputePipelineManager.h"

ParticleUpdater3D::ParticleUpdater3D(DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvUavManager, ComputePipelineManager* computePipelineManager) {
	dxgi_ = dxgi;
	command_ = command;
	srvUavManager_ = srvUavManager;
	computePipelineManager_ = computePipelineManager;

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {

		// 
		// 発生用のリソース作成
		// 
		emitBuffer_[i] = dxgi_->CreateBufferResource(sizeof(ParticleEffectDataForGPU) * kMaxParticleNum);

		emitSrvIdx_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(emitSrvIdx_[i], emitBuffer_[i].Get(), kMaxParticleNum, sizeof(ParticleEffectEmitData));

		emitBuffer_[i]->Map(0, nullptr, reinterpret_cast<void**>(&emitData_[i]));

		emitCount_[i] = 0;

		// 
		// 更新用のリソース作成
		// 
		updateBuffer_[i] = dxgi_->CreateBufferResource(sizeof(ParticleEffectDataForGPU) * kMaxParticleNum, true);

		updateSrvIdx_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(updateSrvIdx_[i], updateBuffer_[i].Get(), kMaxParticleNum, sizeof(ParticleEffectDataForGPU));

		updateUavIdx_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateUavStructuredBuffer(updateUavIdx_[i], updateBuffer_[i].Get(), kMaxParticleNum, sizeof(ParticleEffectDataForGPU));

		// 
		// 描画用のリソース作成
		// 
		drawBuffer_[i] = dxgi_->CreateBufferResource(sizeof(DrawParticleEffectDataForGPU) * kMaxParticleNum, true);

		drawSrvIdx_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(drawSrvIdx_[i], drawBuffer_[i].Get(), kMaxParticleNum, sizeof(DrawParticleEffectDataForGPU));

		drawUavIdx_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateUavStructuredBuffer(drawUavIdx_[i], drawBuffer_[i].Get(), kMaxParticleNum, sizeof(DrawParticleEffectDataForGPU));

		// インスタンス用の変数初期化
		instanceCount_[i] = 0;


		// 
		// データの初期化
		// 
		InitData(i);
	}

}

void ParticleUpdater3D::InitData(uint32_t blendindex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = command_->GetList();

	// ステート遷移
	TransitionResource(updateBuffer_[blendindex].Get(), updateResourceState_, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleInit));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleInit));

	// コマンドを積む
	commandList->SetComputeRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(updateUavIdx_[blendindex]));

	// 実行
	commandList->Dispatch(1, 1, 1);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = updateBuffer_[blendindex].Get();
	commandList->ResourceBarrier(1, &uavBarrier);

}

void ParticleUpdater3D::AddParticle(const ParticleEffectEmitData& emitData) {

}

void ParticleUpdater3D::Update() {

}

uint32_t ParticleUpdater3D::GetInstanceDrawCount(BlendMode mode) const {
	return instanceCount_[static_cast<uint32_t>(mode)];
}

uint32_t ParticleUpdater3D::GetInstancingDrawSrvIndex(BlendMode mode) const {
	return drawSrvIdx_[static_cast<uint32_t>(mode)];
}

void ParticleUpdater3D::TransitionResource(ID3D12Resource* pResource, D3D12_RESOURCE_STATES& current, D3D12_RESOURCE_STATES after) {
	if (current == after)return;

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = current;
	barrier.Transition.StateAfter = after;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	command_->GetList()->ResourceBarrier(1, &barrier);

	current = after;
}
