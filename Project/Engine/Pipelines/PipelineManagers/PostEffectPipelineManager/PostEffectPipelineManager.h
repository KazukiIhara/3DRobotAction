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
#include "Enums/PostEffectPipelineEnum.h"

#include "PostEffectPipelines/CopyPostEffectPipeline/CopyPostEffectPipeline.h"
#include "PostEffectPipelines/GrayscalePostEffectPipeline/GrayscalePostEffectPipeline.h"
#include "PostEffectPipelines/VignettePostEffectPipeline/VignettePostEffectPipeline.h"
#include "PostEffectPipelines/GaussianBlurXPostEffectPipeline/GaussianBlurXPostEffectPipeline.h"
#include "PostEffectPipelines/GaussianBlurYPostEffectPipeline/GaussianBlurYPostEffectPipeline.h"
#include "PostEffectPipelines/RadialBlurPostEffectPipeline/RadialBlurPostEffectPipeline.h"
#include "PostEffectPipelines/DepthOutlinePosteffectPipeline/DepthOutlinePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトパイプラインマネージャ
/// </summary>
class PostEffectPipelineManager {
public:
	PostEffectPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~PostEffectPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(PostEffectType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(PostEffectType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(PostEffectType pipelineState);

	// パイプラインをセット
	void SetPipelineState(PostEffectType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kPostEffectPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> postEffectPipelineStates_[kPostEffectPipelineStateNum][kBlendModeNum];
private:
	// NonePostEffectPipeline
	std::unique_ptr<CopyPostEffectPipeline> copyPostEffectPipeline_ = nullptr;
	// GrayscalePostEffectPipeline
	std::unique_ptr<GrayscalePostEffectPipeline> grayscalePostEffectPipeline_ = nullptr;
	// VignettePostEffectPipeline
	std::unique_ptr<VignettePostEffectPipeline> vignettePostEffectPipeline_ = nullptr;
	// GaussianBlurXPostEffectPipeline
	std::unique_ptr<GaussianBlurXPostEffectPipeline> gaussianBlurXPostEffectPipeline_ = nullptr;
	// GaussianBlurYPostEffectPipeline
	std::unique_ptr<GaussianBlurYPostEffectPipeline> gaussianBlurYPostEffectPipeline_ = nullptr;
	// RadialBlurPostEffectPipeline
	std::unique_ptr<RadialBlurPostEffectPipeline> radialBlurPostEffectPipeline_ = nullptr;
	// DepthPostEffectPipeline
	std::unique_ptr<DepthOutlinePostEffectPipeline> depthOutlinePostEffectPipeline_ = nullptr;
};