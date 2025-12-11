#include "DefferedRenderringPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

DefferedRenderringPipelineManager::DefferedRenderringPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("DefferedRednderringPipelineManager Initialize\n");
}

DefferedRenderringPipelineManager::~DefferedRenderringPipelineManager() {
	Logger::Log("DefferedRednderringPipelineManager Finalize\n");
}

void DefferedRenderringPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	// LightingDefferedRenderringPipeline
	lightingDefferedRenderringPipeline_ = std::make_unique<LightingDefferedRenderringPipeline>(dxgi, shaderCompiler);
	lightingDefferedRenderringPipeline_->Initialize();
	SetRootSignature(DefferedRenderringType::Lighting);
	SetPipelineState(DefferedRenderringType::Lighting);

}

ID3D12RootSignature* DefferedRenderringPipelineManager::GetRootSignature(DefferedRenderringType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* DefferedRenderringPipelineManager::GetPipelineState(DefferedRenderringType pipelineState) {
	return defferedRenderringPipelineStates_[static_cast<uint32_t>(pipelineState)].Get();
}

void DefferedRenderringPipelineManager::SetRootSignature(DefferedRenderringType pipelineState) {
	switch (pipelineState) {
	case DefferedRenderringType::Lighting:
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = lightingDefferedRenderringPipeline_->GetRootSignature();
		break;
	}

}

void DefferedRenderringPipelineManager::SetPipelineState(DefferedRenderringType pipelineState) {
	switch (pipelineState) {
	case DefferedRenderringType::Lighting:
		defferedRenderringPipelineStates_[static_cast<uint32_t>(pipelineState)] = lightingDefferedRenderringPipeline_->GetPipelineState();
		break;
	}
}
