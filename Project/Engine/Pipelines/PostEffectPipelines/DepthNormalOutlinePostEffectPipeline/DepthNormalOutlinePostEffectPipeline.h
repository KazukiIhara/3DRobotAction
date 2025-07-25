#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

class DepthNormalOutlinePostEffectPipeline :public BaseWithParamaterPostEffectPipeline {
public:
	DepthNormalOutlinePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~DepthNormalOutlinePostEffectPipeline()override = default;

private:
	// ルートシグネイチャ作成
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
};