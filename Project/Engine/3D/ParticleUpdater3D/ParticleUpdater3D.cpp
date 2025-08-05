#include "ParticleUpdater3D.h"

#include <cassert>

#include "MAGIAssert/MAGIAssert.h"
#include "DeltaTimer/DeltaTimer.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/ComputePipelineManager/ComputePipelineManager.h"

ParticleUpdater3D::ParticleUpdater3D(DeltaTimer* deltaTimer, DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvUavManager, ComputePipelineManager* computePipelineManager) {
	deltaTimer_ = deltaTimer;
	dxgi_ = dxgi;
	command_ = command;
	srvUavManager_ = srvUavManager;
	computePipelineManager_ = computePipelineManager;

	assert(deltaTimer);
	assert(dxgi_);
	assert(command_);
	assert(srvUavManager_);
	assert(computePipelineManager_);

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
	emitParticleBuffer_ = dxgi_->CreateBufferResource(sizeof(GPUParticleEmitData) * kMaxParticleNum);
	// SRV
	emitSrvIdx_ = srvUavManager_->Allocate();
	srvUavManager_->CreateSrvStructuredBuffer(emitSrvIdx_, emitParticleBuffer_.Get(), kMaxParticleNum, sizeof(GPUParticleEmitData));
	// Map
	emitParticleBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&emitParticleData_));


	// 射出するパーティクルの数を送る
	particleInfoBuffer_ = dxgi_->CreateBufferResource(sizeof(GPUParticleInfo));
	// Map
	particleInfoBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&particleInfo));


	// フリーリストのインデックス
	freeListIdxBuffer_ = dxgi_->CreateBufferResource(sizeof(int32_t), true);
	// UAV
	freeListIdxUavIdx_ = srvUavManager_->Allocate();
	srvUavManager_->CreateUavStructuredBuffer(freeListIdxUavIdx_, freeListIdxBuffer_.Get(), 1, sizeof(int32_t));

	// フリーリスト
	freeListBuffer_ = dxgi_->CreateBufferResource(sizeof(uint32_t) * kMaxParticleNum, true);
	// UAV
	freeListUavIdx_ = srvUavManager_->Allocate();
	srvUavManager_->CreateUavStructuredBuffer(freeListUavIdx_, freeListBuffer_.Get(), kMaxParticleNum, sizeof(uint32_t));


	// パーティクルデータ初期化
	InitData();

}

void ParticleUpdater3D::InitData() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = command_->GetList();

	// ステート遷移
	TransitionResource(particleBuffer_.Get(), currentParticleResourceState_, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// SRVUAVのディスクリプタヒープを設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvUavManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleInit));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleInit));

	// コマンドを積む
	commandList->SetComputeRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(particleUavIdx_));
	commandList->SetComputeRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(freeListIdxUavIdx_));
	commandList->SetComputeRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(freeListUavIdx_));


	// 実行
	commandList->Dispatch(1, 1, 1);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrierParticle{};
	uavBarrierParticle.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierParticle.UAV.pResource = particleBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierParticle);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrierFListIdx{};
	uavBarrierFListIdx.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierFListIdx.UAV.pResource = freeListIdxBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierFListIdx);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrierFList{};
	uavBarrierFList.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierFList.UAV.pResource = freeListBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierFList);

	// パーティクルの発生カウント初期化
	emitCount_ = 0;
	particleInfo->emitCount = emitCount_;
}

void ParticleUpdater3D::AddParticle(const GPUParticleEmitData& emitData) {
	// パーティクル上限検査
	MAGIAssert::Assert(kMaxParticleNum >= emitCount_, "MaxParticleNum !");

	// 参照を取得
	auto& data = emitParticleData_[emitCount_];

	// データを挿入
	data = emitData;

	// 発生数をインクリメント
	emitCount_++;
}


void ParticleUpdater3D::Update() {
	// 
	// パーティクル発生
	// 

	// パーティクルの発生数を入力
	particleInfo->emitCount = emitCount_;
	// デルタタイムを入力
	particleInfo->deltaTime = deltaTimer_->GetDeltaTime();


	// 更新用のステートへ遷移
	TransitionResource(particleBuffer_.Get(), currentParticleResourceState_, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = command_->GetList();

	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleEmit));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleEmit));

	// パラメータを積む

	// パーティクルリスト (UAV)
	commandList->SetComputeRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(particleUavIdx_));
	// 発射されるパーティクルのリスト (SRV)
	commandList->SetComputeRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(emitSrvIdx_));
	// パーティクルに関する定数情報(CBV)
	commandList->SetComputeRootConstantBufferView(2, particleInfoBuffer_->GetGPUVirtualAddress());
	// パーティクルの空きリストインデックス
	commandList->SetComputeRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(freeListIdxUavIdx_));
	// パーティクルの空きリスト
	commandList->SetComputeRootDescriptorTable(4, srvUavManager_->GetDescriptorHandleGPU(freeListUavIdx_));
	// 実行
	commandList->Dispatch(1, 1, 1);


	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = particleBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrier);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrierFListIdx{};
	uavBarrierFListIdx.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierFListIdx.UAV.pResource = freeListIdxBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierFListIdx);

	// UAV 完了保証
	D3D12_RESOURCE_BARRIER uavBarrierFList{};
	uavBarrierFList.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierFList.UAV.pResource = freeListBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierFList);

	//
	// パーティクル更新
	// 

	// パイプライン設定
	commandList->SetComputeRootSignature(computePipelineManager_->GetRootSignature(ComputePipelineStateType::ParticleUpdate));
	commandList->SetPipelineState(computePipelineManager_->GetPipelineState(ComputePipelineStateType::ParticleUpdate));


	// パラメータを積む

	// パーティクルリスト (UAV)
	commandList->SetComputeRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(particleUavIdx_));
	// パーティクルに関する定数情報(CBV)
	commandList->SetComputeRootConstantBufferView(1, particleInfoBuffer_->GetGPUVirtualAddress());
	// パーティクルの空きリストインデックス
	commandList->SetComputeRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(freeListIdxUavIdx_));
	// パーティクルの空きリスト
	commandList->SetComputeRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(freeListUavIdx_));

	// 実行
	commandList->Dispatch(1, 1, 1);

	// UAV 完了保証
	uavBarrierFListIdx.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierFListIdx.UAV.pResource = freeListIdxBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierFListIdx);

	// UAV 完了保証
	uavBarrierFList.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrierFList.UAV.pResource = freeListBuffer_.Get();
	commandList->ResourceBarrier(1, &uavBarrierFList);

	// 描画用のステートへ遷移 
	TransitionResource(particleBuffer_.Get(), currentParticleResourceState_, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);

	// 次のフレーム用に初期化
	emitCount_ = 0;
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
