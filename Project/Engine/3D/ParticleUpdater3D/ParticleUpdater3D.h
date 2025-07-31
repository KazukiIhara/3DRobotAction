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

	void InitData(uint32_t blendindex);

	void AddParticle(const ParticleEffectEmitData& emitData);

	void Update();

	uint32_t GetInstanceDrawCount(BlendMode mode)const;

	uint32_t GetInstancingDrawSrvIndex(BlendMode mode)const;

private:
	// リソース遷移
	void TransitionResource(ID3D12Resource* pResource, D3D12_RESOURCE_STATES& current, D3D12_RESOURCE_STATES after);

private:
	// パーティクル発生用のリソース(SRV)
	ComPtr<ID3D12Resource> emitBuffer_[static_cast<uint32_t>(BlendMode::Num)];
	// 発生用SRVインデックス
	uint32_t emitSrvIdx_[static_cast<uint32_t>(BlendMode::Num)];
	// パーティクル発生用のデータ
	ParticleEffectDataForGPU* emitData_[static_cast<uint32_t>(BlendMode::Num)];
	// パーティクルの発生数
	uint32_t emitCount_[static_cast<uint32_t>(BlendMode::Num)];


	// パーティクル更新用のリソース(SRV/UAV)
	ComPtr<ID3D12Resource> updateBuffer_[static_cast<uint32_t>(BlendMode::Num)];
	// 更新用SRVインデックス
	uint32_t updateSrvIdx_[static_cast<uint32_t>(BlendMode::Num)];
	// 更新用UAVインデックス
	uint32_t updateUavIdx_[static_cast<uint32_t>(BlendMode::Num)];
	// リソースステート
	D3D12_RESOURCE_STATES updateResourceState_ = D3D12_RESOURCE_STATE_COMMON;


	// パーティクル描画用のリソース(SRV/UAV)
	ComPtr<ID3D12Resource> drawBuffer_[static_cast<uint32_t>(BlendMode::Num)];
	// 描画用SRVインデックス
	uint32_t drawSrvIdx_[static_cast<uint32_t>(BlendMode::Num)];
	// 描画用UAVインデックス
	uint32_t drawUavIdx_[static_cast<uint32_t>(BlendMode::Num)];
	// リソースステート
	D3D12_RESOURCE_STATES drawResourceState_ = D3D12_RESOURCE_STATE_COMMON;


	// instanceの数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* command_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	ComputePipelineManager* computePipelineManager_ = nullptr;

};