#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"


#include "Common/ParticleEffectCommon.h"

/// <summary>
/// パーティクル描画用パイプライン
/// </summary>
class ParticleEffectGraphicsPipeline : public BaseGraphicsPipeline {
public:
	ParticleEffectGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~ParticleEffectGraphicsPipeline()override = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject()override;

	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;
};