#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

/// <summary>
/// 深度比較でアウトラインを出す
/// </summary>
class DepthOutlinePostEffectPipeline :public BaseWithParamaterPostEffectPipeline {
public:
	DepthOutlinePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~DepthOutlinePostEffectPipeline()override = default;

private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};