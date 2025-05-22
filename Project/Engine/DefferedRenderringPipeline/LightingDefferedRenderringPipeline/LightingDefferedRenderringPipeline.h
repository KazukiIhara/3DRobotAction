#pragma once

#include "DefferedRenderringPipeline/BaseDefferedRenderringPipeline/BaseDefferedRenderringPipeline.h"

/// <summary>
/// ディファードレンダリングでライティングをするパイプライン
/// </summary>
class LightingDefferedRenderringPipeline :public BaseDefferedRenderringPipeline {
public:
	LightingDefferedRenderringPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~LightingDefferedRenderringPipeline()override;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject()override;
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting()override;
	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting()override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;

};