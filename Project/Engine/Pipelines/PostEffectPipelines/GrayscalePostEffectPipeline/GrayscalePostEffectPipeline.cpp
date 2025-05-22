#include "GrayscalePostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

GrayscalePostEffectPipeline::GrayscalePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseNoParamaterPostEffectPipeline(dxgi, shaderCompiler) {}

GrayscalePostEffectPipeline::~GrayscalePostEffectPipeline() {}

void GrayscalePostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Grayscale/Grayscale.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Grayscale/Grayscale.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}