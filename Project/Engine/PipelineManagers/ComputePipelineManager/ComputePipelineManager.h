#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "Enums/ComputePipelineEnum.h"

#include "ComputePipelines/SkinningComputePipeline/SkinningComputePipeline.h"

class DXGI;

class ShaderCompiler;

class ComputePipelineManager {
public:
	ComputePipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~ComputePipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(ComputePipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(ComputePipelineStateType pipelineState);

	void SetRootSignature(ComputePipelineStateType pipelineState);

	void SetPipelineState(ComputePipelineStateType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kComputePipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> computePipelineStates_[kComputePipelineStateNum];
private:
	// Skinning用のComputePipeline
	std::unique_ptr<SkinningComputePipeline> skinningComputePipeline_ = nullptr;

};