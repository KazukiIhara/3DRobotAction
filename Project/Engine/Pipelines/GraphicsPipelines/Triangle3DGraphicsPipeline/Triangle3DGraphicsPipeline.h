#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

#include "Structs/Primitive3DStruct.h"

/// <summary>
/// 3D三角形描画用のパイプライン
/// </summary>
class Triangle3DGraphicsPipeline: public BaseGraphicsPipeline {
public:
	Triangle3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~Triangle3DGraphicsPipeline() override = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting()override;
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSettingBlend(uint32_t blendModeNum);
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;
};
