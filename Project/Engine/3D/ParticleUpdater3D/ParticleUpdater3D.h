#pragma once

// C++
#include <cstdint>
#include <vector>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Common/ParticleEffectCommon.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class SRVUAVManager;
class ComputePipelineManager;

/// <summary>
/// 3Dパーティクル更新クラス
/// </summary>
class ParticleUpdater3D {
public:
	ParticleUpdater3D(DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvUavManager, ComputePipelineManager* computePipelineManager);
	~ParticleUpdater3D() = default;

	void InitData();

	void AddParticle(const GPUParticleEmitData& emitData);

	void Update();

	uint32_t GetInstancingSrvIndex()const;

private:
	// リソース遷移
	void TransitionResource(ID3D12Resource* pResource, D3D12_RESOURCE_STATES& current, D3D12_RESOURCE_STATES after);

private:
	// パーティクル全部
	ComPtr<ID3D12Resource> particleBuffer_;
	uint32_t particleSrvIdx_;
	uint32_t particleUavIdx_;
	D3D12_RESOURCE_STATES currentParticleResourceState_ = D3D12_RESOURCE_STATE_COMMON;

	// 射出するパーティクル
	ComPtr<ID3D12Resource> emitParticleBuffer_;
	GPUParticleEmitData* emitParticleData_ = nullptr;
	uint32_t emitSrvIdx_;

	// 射出するパーティクルの数を送る
	ComPtr<ID3D12Resource> emitCountBuffer_;
	GPUParticleEmitCount* emitCountData_ = nullptr;
	uint32_t emitCount_;

	// AliveBuffer
	ComPtr<ID3D12Resource> aliveListBuffer_[2];
	// DeadList
	ComPtr<ID3D12Resource> deadListBuffer_;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* command_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	ComputePipelineManager* computePipelineManager_ = nullptr;

};