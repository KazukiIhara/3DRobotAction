#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"



namespace magi {
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

} // namespace magi
