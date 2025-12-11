#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

namespace magi {
	class RandomPostEffectPipeline:public BaseWithParamaterPostEffectPipeline {
	public:
		RandomPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
		~RandomPostEffectPipeline()override = default;

	private:
		// シェーダーをコンパイルする
		void CompileShaders()override;
	};
}