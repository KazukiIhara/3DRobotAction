#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

#include "Structs/SpriteStruct.h"

/// <summary>
/// スプライト描画用パイプライン
/// </summary>
class SpriteGraphicsPipeline: public BaseGraphicsPipeline {
public:
	SpriteGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~SpriteGraphicsPipeline()override = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// PSO作成
	void CreateGraphicsPipelineObject();
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;
};