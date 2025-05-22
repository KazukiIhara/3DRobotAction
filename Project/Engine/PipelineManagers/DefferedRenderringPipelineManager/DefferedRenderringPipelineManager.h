#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// 前方宣言
class DXGI;
class ShaderCompiler;

// MyHedder
#include "Enums/DefferedRenderringPipelineEnum.h"

#include "DirectX/ComPtr/ComPtr.h"

#include "DefferedRenderringPipeline/LightingDefferedRenderringPipeline/LightingDefferedRenderringPipeline.h"

/// <summary>
/// ディファードレンダリングのパイプラインマネージャ
/// </summary>
class DefferedRenderringPipelineManager {
public:
	DefferedRenderringPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~DefferedRenderringPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(DefferedRenderringType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(DefferedRenderringType pipelineState);

	// ルートシグネイチャをセット
	void SetRootSignature(DefferedRenderringType pipelineState);

	// パイプラインをセット
	void SetPipelineState(DefferedRenderringType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kDefferedRenderringPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> defferedRenderringPipelineStates_[kDefferedRenderringPipelineStateNum];
private:
	// LightingDefferedRenderringPipeline
	std::unique_ptr<LightingDefferedRenderringPipeline> lightingDefferedRenderringPipeline_ = nullptr;
};