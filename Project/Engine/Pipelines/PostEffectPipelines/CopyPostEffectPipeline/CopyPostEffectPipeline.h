#pragma once

#include "PostEffectPipelines/BaseNoParamaterPostEffectPipeline/BaseNoParamaterPostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class CopyPostEffectPipeline:public BaseNoParamaterPostEffectPipeline {
public:
	CopyPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~CopyPostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};