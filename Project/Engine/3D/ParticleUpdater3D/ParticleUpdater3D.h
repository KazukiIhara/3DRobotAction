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
	ParticleUpdater3D();
	~ParticleUpdater3D() = default;

	void AddParticle(const ParticleEffectEmitData& emitData);

	void Update();

	uint32_t GetInstanceDrawCount(BlendMode mode)const;

	uint32_t GetInstancingDrawSrvIndex(BlendMode mode)const;

private:
	// パーティクル発生用のリソース(SRV)
	ComPtr<ID3D12Resource> instancingEmitResource_[static_cast<uint32_t>(BlendMode::Num)];
	// パーティクル発生用のデータ
	ParticleEffectDataForGPU* instancingEmitData_[static_cast<uint32_t>(BlendMode::Num)];

	// 発生用SRVインデックス
	uint32_t instancingEmitSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];


	// パーティクル更新用のリソース(SRV/UAV)
	ComPtr<ID3D12Resource> instancingUpdateResource_[static_cast<uint32_t>(BlendMode::Num)];
	// 更新用SRVインデックス
	uint32_t instancingUpdateSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];
	// 更新用UAVインデックス
	uint32_t instancingUpdateUavIndex_[static_cast<uint32_t>(BlendMode::Num)];


	// パーティクル描画用のリソース(SRV/UAV)
	ComPtr<ID3D12Resource> instancingDrawResource_[static_cast<uint32_t>(BlendMode::Num)];
	// 描画用SRVインデックス
	uint32_t instancingDrawSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];
	// 描画用UAVインデックス
	uint32_t instancingDrawUavIndex_[static_cast<uint32_t>(BlendMode::Num)];


	// instance描画する際に使う変数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];

};