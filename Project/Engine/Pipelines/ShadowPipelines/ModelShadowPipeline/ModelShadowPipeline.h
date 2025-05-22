#pragma once

#include "ShadowPipelines/BaseShadowPipeline/BaseShadowPipeline.h"

/// <summary>
/// モデルのシャドウマップ描画用パイプライン
/// </summary>
class ModelShadowPipeline :public BaseShadowPipeline {
public:
	ModelShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~ModelShadowPipeline() = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};