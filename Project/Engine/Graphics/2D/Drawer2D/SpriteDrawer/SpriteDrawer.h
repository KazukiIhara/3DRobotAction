#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/ColorStruct.h"
#include "Structs/SpriteStruct.h"
#include "Enums/BlendModeEnum.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera2DManager;

/// <summary>
/// スプライト描画クラス
/// </summary>
class SpriteDrawer {
public:
	SpriteDrawer(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera2DManager* camera2DManager
	);
	~SpriteDrawer();

	void Update();
	void DrawFront(BlendMode blendMode);
	void DrawBack(BlendMode blendMode);

	void AddSprite(
		const SpriteData& data,
		const SpriteMaterialData& material
	);

private:
	SpriteDataForGPU ComputeSpriteDataForGPU(const SpriteData& data, const SpriteMaterialData& material);
private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera2DManager(Camera2DManager* camera2DManager);

private:
	// インスタンス最大数
	const uint32_t NumMaxInstance = 262144;

	//===============================
	// Front
	//===============================

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResourceFront_[static_cast<uint32_t>(BlendMode::Num)];
	// instancing描画用のデータ
	SpriteDataForGPU* instancingDataFront_[static_cast<uint32_t>(BlendMode::Num)];

	// SpriteSrvIndex
	uint32_t instancingSrvIndexFront_[static_cast<uint32_t>(BlendMode::Num)];

	// instance描画する際に使う変数
	uint32_t instanceCountFront_[static_cast<uint32_t>(BlendMode::Num)];

	// 現在のインデックス
	uint32_t currentIndexFront_[static_cast<uint32_t>(BlendMode::Num)];

	//===============================
	// Back
	//===============================

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResourceBack_[static_cast<uint32_t>(BlendMode::Num)];
	// instancing描画用のデータ
	SpriteDataForGPU* instancingDataBack_[static_cast<uint32_t>(BlendMode::Num)];

	// SpriteSrvIndex
	uint32_t instancingSrvIndexBack_[static_cast<uint32_t>(BlendMode::Num)];

	// instance描画する際に使う変数
	uint32_t instanceCountBack_[static_cast<uint32_t>(BlendMode::Num)];

	// 現在のインデックス
	uint32_t currentIndexBack_[static_cast<uint32_t>(BlendMode::Num)];

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera2DManager* camera2DManager_ = nullptr;
};