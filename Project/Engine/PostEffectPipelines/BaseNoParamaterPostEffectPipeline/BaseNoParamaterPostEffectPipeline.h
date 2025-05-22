#pragma once

#include "PostEffectPipelines/BasePostEffectPipeline/BasePostEffectPipeline.h"

/// <summary>
/// 追加情報をシェーダーに送らないポストエフェクトのパイプライン基底クラス
/// </summary>
class BaseNoParamaterPostEffectPipeline:public BasePostEffectPipeline {
public:
	BaseNoParamaterPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~BaseNoParamaterPostEffectPipeline()override;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders() = 0;
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
