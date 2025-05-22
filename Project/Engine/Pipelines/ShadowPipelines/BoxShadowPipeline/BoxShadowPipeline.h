#pragma once

#include "ShadowPipelines/BaseShadowPipeline/BaseShadowPipeline.h"

/// <summary>
/// ボックスのシャドウマップ描画用パイプライン
/// </summary>
class BoxShadowPipeline :public BaseShadowPipeline {
public:
	BoxShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~BoxShadowPipeline() = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};