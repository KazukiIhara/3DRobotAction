#include "ComputePipelineManager.h"

#include "Logger/Logger.h"

ComputePipelineManager::ComputePipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("ComputePipelineManager Initialize\n");
}

ComputePipelineManager::~ComputePipelineManager() {
	Logger::Log("ComputePipelineManager Finalize\n");
}

void ComputePipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	// Skinning用のグラフィックスパイプラインを生成、初期化
	skinningComputePipeline_ = std::make_unique<SkinningComputePipeline>(dxgi, shaderCompiler);
	SetRootSignature(ComputePipelineStateType::Skinning);
	SetPipelineState(ComputePipelineStateType::Skinning);

	// InitParticle
	initParticlePipeline_ = std::make_unique<InitParticleComputePipeline>(dxgi, shaderCompiler);
	SetRootSignature(ComputePipelineStateType::ParticleInit);
	SetPipelineState(ComputePipelineStateType::ParticleInit);

	// 他のパイプランはここに追加

}

ID3D12RootSignature* ComputePipelineManager::GetRootSignature(ComputePipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* ComputePipelineManager::GetPipelineState(ComputePipelineStateType pipelineState) {
	return computePipelineStates_[static_cast<uint32_t>(pipelineState)].Get();
}

void ComputePipelineManager::SetRootSignature(ComputePipelineStateType pipelineState) {
	switch (pipelineState) {
	case ComputePipelineStateType::Skinning:
		// Skinning
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = skinningComputePipeline_->GetRootSignature();
		break;
	case ComputePipelineStateType::ParticleInit:
		// InitParticle
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = initParticlePipeline_->GetRootSignature();
		break;
	}
}

void ComputePipelineManager::SetPipelineState(ComputePipelineStateType pipelineState) {
	switch (pipelineState) {
	case ComputePipelineStateType::Skinning:
		computePipelineStates_[static_cast<uint32_t>(pipelineState)] = skinningComputePipeline_->GetPipelineState();
		break;
	case ComputePipelineStateType::ParticleInit:
		computePipelineStates_[static_cast<uint32_t>(pipelineState)] = initParticlePipeline_->GetPipelineState();
		break;
	}
}
