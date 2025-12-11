#pragma once

#include "PostEffectPipelines/BaseNoParamaterPostEffectPipeline/BaseNoParamaterPostEffectPipeline.h"


/// <summary>
/// グレースケール
/// </summary>

namespace magi {
class GrayscalePostEffectPipeline:public BaseNoParamaterPostEffectPipeline {
public:
	GrayscalePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~GrayscalePostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};

} // namespace magi
