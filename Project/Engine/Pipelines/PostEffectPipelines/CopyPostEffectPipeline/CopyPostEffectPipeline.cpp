#include "CopyPostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

CopyPostEffectPipeline::CopyPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseNoParamaterPostEffectPipeline(dxgi, shaderCompiler) {
}

CopyPostEffectPipeline::~CopyPostEffectPipeline() {}

void CopyPostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Copy/Copy.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/Copy/Copy.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}