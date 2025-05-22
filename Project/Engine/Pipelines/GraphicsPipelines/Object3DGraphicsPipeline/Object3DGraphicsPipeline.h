#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// 3D描画パイプライン
/// </summary>
class Object3DGraphicsPipeline:public BaseGraphicsPipeline {
public:
	Object3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~Object3DGraphicsPipeline()override;
private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject()override;
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting()override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;

};