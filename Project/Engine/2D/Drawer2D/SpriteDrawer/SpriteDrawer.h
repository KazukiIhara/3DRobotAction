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


namespace magi {
class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera2DManager;

/// <summary>
/// スプライト描画クラス
/// </summary>
class SpriteDrawer {
private:
	struct Data {
		ComPtr<ID3D12Resource> instancingResource[static_cast<uint32_t>(BlendMode::Num)];
		SpriteDataForGPU* instancingData[static_cast<uint32_t>(BlendMode::Num)];
		uint32_t instancingSrvIndex[static_cast<uint32_t>(BlendMode::Num)];
		uint32_t instanceCount[static_cast<uint32_t>(BlendMode::Num)];
		uint32_t currentIndex[static_cast<uint32_t>(BlendMode::Num)];
	};
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
	const uint32_t NumMaxInstance = 65536;

	// 正面用データ
	Data front_;

	// 背面用データ
	Data back_;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera2DManager* camera2DManager_ = nullptr;
};

} // namespace magi
