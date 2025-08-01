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

	// 
	// リソース作成
	// 

	// パーティクル
	particleBuffer_ = dxgi_->CreateBufferResource(sizeof(GPUParticle) * kMaxParticleNum, true);
	// SRV
	particleSrvIdx_ = srvUavManager_->Allocate();
	srvUavManager_->CreateSrvStructuredBuffer(particleSrvIdx_, particleBuffer_.Get(), kMaxParticleNum, sizeof(GPUParticle));
	// UAV
	particleUavIdx_ = srvUavManager_->Allocate();
	srvUavManager_->CreateUavStructuredBuffer(particleUavIdx_, particleBuffer_.Get(), kMaxParticleNum, sizeof(GPUParticle));


	// 射出するパーティクル
	emitParticleBuffer_ = dxgi_->CreateBufferResource(sizeof(GPUParticleEmitData) * kMaxParticleNum, true);
	// SRV
	emitSrvIdx_ = srvUavManager_->Allocate();
	srvUavManager_->CreateSrvStructuredBuffer(emitSrvIdx_, emitParticleBuffer_.Get(), kMaxParticleNum, sizeof(GPUParticleEmitData));


	// パーティクルデータ初期化
	InitData();

	// パーティクルの発生カウント初期化
	particleEmitCount_ = 0;

}

void ParticleUpdater3D::InitData() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = command_->GetList();

	// ステート遷移
	TransitionResource(particleBuffer_.Get(), currentParticleResourceState_, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleInit));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleInit));

	// コマンドを積む
	commandList->SetComputeRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(particleUavIdx_));

	// 実行
	commandList->Dispatch(1, 1, 1);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = particleBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrier);

}

void ParticleUpdater3D::AddParticle(const GPUParticleEmitData& emitData) {
	// 参照を取得
	auto& data = emitParticleData_[particleEmitCount_];

	// データを挿入
	data = emitData;

	// 発生数をインクリメント
	particleEmitCount_++;
}


void ParticleUpdater3D::Update() {
	// 
	// パーティクル発生
	// 

	// 更新用のステートへ遷移
	TransitionResource(particleBuffer_.Get(), currentParticleResourceState_, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = command_->GetList();
	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleEmit));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleEmit));

	// パラメータを積む


	// 実行
	commandList->Dispatch(1, 1, 1);


	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = particleBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrier);


	//
	// パーティクル更新
	// 

	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleUpdate));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleUpdate));

	// パラメータを積む


	// 実行
	commandList->Dispatch(1, 1, 1);

	// 描画用のステートへ遷移 
	TransitionResource(particleBuffer_.Get(), currentParticleResourceState_, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);

	// 発生数を初期化
	particleEmitCount_ = 0;
}

uint32_t ParticleUpdater3D::GetInstancingSrvIndex() const {
	return particleSrvIdx_;
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
