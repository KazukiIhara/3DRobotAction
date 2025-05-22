#pragma once

#include "ShadowPipelines/BaseShadowPipeline/BaseShadowPipeline.h"

/// <summary>
/// 球体のシャドウマップ描画用パイプライン
/// </summary>
class SphereShadowPipeline :public BaseShadowPipeline {
public:
	SphereShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~SphereShadowPipeline() = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
