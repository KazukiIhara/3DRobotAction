#include "RandomPostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

RandomPostEffectPipeline::RandomPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseWithParamaterPostEffectPipeline(dxgi, shaderCompiler) {}

void RandomPostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Random/Random.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Random/Random.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}
