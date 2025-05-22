#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

#include "Structs/Primitive3DStruct.h"

/// <summary>
/// 3D板ポリ描画用のパイプライン
/// </summary>
class Plane3DGraphicsPipeline : public BaseGraphicsPipeline {
public:
	Plane3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~Plane3DGraphicsPipeline()override = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;

	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;
};