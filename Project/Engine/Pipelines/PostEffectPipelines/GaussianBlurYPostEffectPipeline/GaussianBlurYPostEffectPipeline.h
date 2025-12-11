#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

/// <summary>
/// Y軸ガウシアンブラー
/// </summary>
class GaussianBlurYPostEffectPipeline:public BaseWithParamaterPostEffectPipeline {
public:
	GaussianBlurYPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~GaussianBlurYPostEffectPipeline()override;

private:
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
