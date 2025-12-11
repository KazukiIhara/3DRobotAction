#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

#include "Structs/Primitive3DStruct.h"

/// <summary>
/// 3Dシリンダー描画用のパイプライン
/// </summary>
class Cylinder3DGraphicsPipeline : public BaseGraphicsPipeline {
public:
	Cylinder3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~Cylinder3DGraphicsPipeline()override = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting()override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;
};