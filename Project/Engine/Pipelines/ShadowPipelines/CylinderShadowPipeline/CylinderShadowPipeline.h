#pragma once

#include "ShadowPipelines/BaseShadowPipeline/BaseShadowPipeline.h"

/// <summary>
/// シリンダーのシャドウマップ描画用パイプライン
/// </summary>
class CylinderShadowPipeline :public BaseShadowPipeline {
public:
	CylinderShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~CylinderShadowPipeline() = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};