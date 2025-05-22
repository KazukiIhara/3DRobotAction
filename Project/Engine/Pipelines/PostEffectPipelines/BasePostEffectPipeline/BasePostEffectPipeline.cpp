#include "BasePostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

BasePostEffectPipeline::BasePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);
}

BasePostEffectPipeline::~BasePostEffectPipeline() {
}

void BasePostEffectPipeline::Initialize() {
	CreateRootSignature();
	CompileShaders();
	CreateGraphicsPipelineObject();
}

ID3D12RootSignature* BasePostEffectPipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* BasePostEffectPipeline::GetPipelineState(BlendMode blendMode) {
	return pipelineState_[static_cast<uint32_t>(blendMode)].Get();
}

void BasePostEffectPipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void BasePostEffectPipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
