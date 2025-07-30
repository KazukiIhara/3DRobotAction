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
	// パーティクル発生用のリソース


	// instancing更新用のリソース

	// instancing更新用のデータ

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingDrawResource_[static_cast<uint32_t>(BlendMode::Num)];
	// instancing描画用のデータ
	DrawParticleEffectDataForGPU* instancingDrawData_[static_cast<uint32_t>(BlendMode::Num)];

	// instance描画する際に使う変数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];

	// instancing描画用SRVインデックス
	uint32_t instancingDrawSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];
};