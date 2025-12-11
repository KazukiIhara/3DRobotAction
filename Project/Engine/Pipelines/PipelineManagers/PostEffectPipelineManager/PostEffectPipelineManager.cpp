#include "PostEffectPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

PostEffectPipelineManager::PostEffectPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("PostEffectPipelineManager Initialize\n");
}

PostEffectPipelineManager::~PostEffectPipelineManager() {
	Logger::Log("PostEffectPipelineManager Finalize\n");
}

void PostEffectPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	// Copyのパイプラインを生成、初期化
	copyPostEffectPipeline_ = std::make_unique<CopyPostEffectPipeline>(dxgi, shaderCompiler);
	copyPostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Copy);
	SetPipelineState(PostEffectType::Copy);

	// Grayscaleのパイプラインを生成、初期化
	grayscalePostEffectPipeline_ = std::make_unique<GrayscalePostEffectPipeline>(dxgi, shaderCompiler);
	grayscalePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Grayscale);
	SetPipelineState(PostEffectType::Grayscale);

	// Vignetteのパイプラインを生成、初期化
	vignettePostEffectPipeline_ = std::make_unique<VignettePostEffectPipeline>(dxgi, shaderCompiler);
	vignettePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Vignette);
	SetPipelineState(PostEffectType::Vignette);

	// GaussianBlurXのパイプラインを生成、初期化
	gaussianBlurXPostEffectPipeline_ = std::make_unique<GaussianBlurXPostEffectPipeline>(dxgi, shaderCompiler);
	gaussianBlurXPostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::GaussianX);
	SetPipelineState(PostEffectType::GaussianX);

	// GaussianBlurYのパイプラインを生成、初期化
	gaussianBlurYPostEffectPipeline_ = std::make_unique<GaussianBlurYPostEffectPipeline>(dxgi, shaderCompiler);
	gaussianBlurYPostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::GaussianY);
	SetPipelineState(PostEffectType::GaussianY);

	// RadialBlurのパイプラインを生成、初期化
	radialBlurPostEffectPipeline_ = std::make_unique<RadialBlurPostEffectPipeline>(dxgi, shaderCompiler);
	radialBlurPostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::RadialBlur);
	SetPipelineState(PostEffectType::RadialBlur);

	// LuminanceOutlineのパイプラインを生成、初期化
	luminanceOutlinePostEffectPipeline_ = std::make_unique<LuminanceOutlinePostEffectPipeline>(dxgi, shaderCompiler);
	luminanceOutlinePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::LuminanceOutline);
	SetPipelineState(PostEffectType::LuminanceOutline);

	// DepthOutlineのパイプラインを生成、初期化
	depthOutlinePostEffectPipeline_ = std::make_unique<DepthOutlinePostEffectPipeline>(dxgi, shaderCompiler);
	depthOutlinePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::DepthOutline);
	SetPipelineState(PostEffectType::DepthOutline);

	// DepthNormalOutlineのパイプラインを生成、初期化
	depthNormalOutlinePostEffectPipeline_ = std::make_unique<DepthNormalOutlinePostEffectPipeline>(dxgi, shaderCompiler);
	depthNormalOutlinePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::DepthNormalOutline);
	SetPipelineState(PostEffectType::DepthNormalOutline);

	// Randomのパイプラインを生成、初期化
	randomPostEffectPipeline_ = std::make_unique<RandomPostEffectPipeline>(dxgi, shaderCompiler);
	randomPostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Random);
	SetPipelineState(PostEffectType::Random);

}

ID3D12RootSignature* PostEffectPipelineManager::GetRootSignature(PostEffectType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* PostEffectPipelineManager::GetPipelineState(PostEffectType pipelineState, BlendMode blendMode) {
	return postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][static_cast<uint32_t>(blendMode)].Get();
}

void PostEffectPipelineManager::SetRootSignature(PostEffectType pipelineState) {
	// パイプラインごとに対応するルートシグネイチャを設定
	switch (pipelineState) {
		case PostEffectType::Copy:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = copyPostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::Grayscale:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = grayscalePostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::Vignette:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = vignettePostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::GaussianX:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = gaussianBlurXPostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::GaussianY:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = gaussianBlurYPostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::RadialBlur:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = radialBlurPostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::LuminanceOutline:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = luminanceOutlinePostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::DepthOutline:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = depthOutlinePostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::DepthNormalOutline:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = depthNormalOutlinePostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectType::Random:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = randomPostEffectPipeline_->GetRootSignature();
			break;
	}

}

void PostEffectPipelineManager::SetPipelineState(PostEffectType pipelineState) {
	// パイプラインごとに対応するパイプラインステートを設定
	switch (pipelineState) {
		case PostEffectType::Copy:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = copyPostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::Grayscale:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = grayscalePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::Vignette:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = vignettePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::GaussianX:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = gaussianBlurXPostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::GaussianY:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = gaussianBlurYPostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::RadialBlur:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = radialBlurPostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::LuminanceOutline:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = luminanceOutlinePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::DepthOutline:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = depthOutlinePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::DepthNormalOutline:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = depthNormalOutlinePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectType::Random:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = randomPostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
	}
}
