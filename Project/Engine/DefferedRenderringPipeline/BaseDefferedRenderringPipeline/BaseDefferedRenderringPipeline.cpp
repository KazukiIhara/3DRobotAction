#include "BaseDefferedRenderringPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

BaseDefferedRenderringPipeline::BaseDefferedRenderringPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);
}

BaseDefferedRenderringPipeline::~BaseDefferedRenderringPipeline() {
}

void BaseDefferedRenderringPipeline::Initialize() {
	CreateRootSignature();
	CompileShaders();
	CreateGraphicsPipelineObject();
}

ID3D12RootSignature* BaseDefferedRenderringPipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* BaseDefferedRenderringPipeline::GetPipelineState() {
	return pipelineState_.Get();
}

void BaseDefferedRenderringPipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void BaseDefferedRenderringPipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
