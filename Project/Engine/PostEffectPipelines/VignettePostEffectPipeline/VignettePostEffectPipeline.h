#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

/// <summary>
/// ビネット
/// </summary>
class VignettePostEffectPipeline :public BaseWithParamaterPostEffectPipeline {
public:
	VignettePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~VignettePostEffectPipeline()override;


private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
