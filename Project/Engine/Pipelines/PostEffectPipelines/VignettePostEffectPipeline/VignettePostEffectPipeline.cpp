#include "VignettePostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

VignettePostEffectPipeline::VignettePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseWithParamaterPostEffectPipeline(dxgi, shaderCompiler) {}

VignettePostEffectPipeline::~VignettePostEffectPipeline() {}

void VignettePostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Vignette/Vignette.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Vignette/Vignette.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}