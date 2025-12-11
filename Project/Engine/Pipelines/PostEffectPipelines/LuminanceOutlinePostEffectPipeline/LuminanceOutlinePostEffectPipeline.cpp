#include "LuminanceOutlinePostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

LuminanceOutlinePostEffectPipeline::LuminanceOutlinePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseWithParamaterPostEffectPipeline(dxgi, shaderCompiler) {
}

void LuminanceOutlinePostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/LuminanceOutline/LuminanceOutline.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/LuminanceOutline/LuminanceOutline.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}
