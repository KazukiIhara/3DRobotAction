#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

/// <summary>
/// ラジアルブラー
/// </summary>
class RadialBlurPostEffectPipeline:public BaseWithParamaterPostEffectPipeline {
public:
	RadialBlurPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~RadialBlurPostEffectPipeline()override;


private:
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};