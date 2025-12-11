#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

/// <summary>
/// 輝度比較によるアウトライン
/// </summary>
class LuminanceOutlinePostEffectPipeline :public BaseWithParamaterPostEffectPipeline {
public:
	LuminanceOutlinePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~LuminanceOutlinePostEffectPipeline()override = default;

private:
	// シェーダーをコンパイル
	void CompileShaders()override;
};