#include "GraphicsPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

GraphicsPipelineManager::GraphicsPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("GraphicsPipelineManager Initialize\n");
}

GraphicsPipelineManager::~GraphicsPipelineManager() {
	Logger::Log("GraphicsPipelineManager Finalize\n");
}

void GraphicsPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	// スプライト描画用のグラフィックスパイプラインを生成、初期化
	spriteGraphicsPipeline_ = std::make_unique<SpriteGraphicsPipeline>(dxgi, shaderCompiler);
	spriteGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Sprite);
	SetPipelineState(GraphicsPipelineStateType::Sprite);

	// 3Dラインのグラフィックスパイプラインを生成、初期化
	line3DGraphicsPipeline_ = std::make_unique<Line3DGraphicsPipeline>(dxgi, shaderCompiler);
	line3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Line3D);
	SetPipelineState(GraphicsPipelineStateType::Line3D);

	// 3D三角形のグラフィックスパイプラインを生成、初期化
	triangle3DGraphicsPipeline_ = std::make_unique<Triangle3DGraphicsPipeline>(dxgi, shaderCompiler);
	triangle3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Triangle3D);
	SetPipelineState(GraphicsPipelineStateType::Triangle3D);

	// 3D板ポリのグラフィックスパイプラインを生成、初期か
	plane3DGraphicsPipeline_ = std::make_unique<Plane3DGraphicsPipeline>(dxgi, shaderCompiler);
	plane3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Plane3D);
	SetPipelineState(GraphicsPipelineStateType::Plane3D);

	// 3Dボックスのグラフィックスパイプラインを生成、初期化
	box3DGraphicsPipeline_ = std::make_unique<Box3DGraphicsPipeline>(dxgi, shaderCompiler);
	box3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Box3D);
	SetPipelineState(GraphicsPipelineStateType::Box3D);

	// 3D球体のグラフィックスパイプラインを生成、初期化
	sphere3DGraphicsPipeline_ = std::make_unique<Sphere3DGraphicsPipeline>(dxgi, shaderCompiler);
	sphere3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Sphere3D);
	SetPipelineState(GraphicsPipelineStateType::Sphere3D);

	// 3Dリングのグラフィックスパイプラインを生成、初期化
	ring3DGraphicsPipeline_ = std::make_unique<Ring3DGraphicsPipeline>(dxgi, shaderCompiler);
	ring3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Ring3D);
	SetPipelineState(GraphicsPipelineStateType::Ring3D);

	// 3Dシリンダーのグラフィックスパイプラインを生成、初期化
	cylinder3DGraphicsPipeline_ = std::make_unique<Cylinder3DGraphicsPipeline>(dxgi, shaderCompiler);
	cylinder3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Cylinder3D);
	SetPipelineState(GraphicsPipelineStateType::Cylinder3D);

	// Modelのグラフィックスパイプラインを生成、初期化
	model3DGraphicsPipeline_ = std::make_unique<Model3DGraphicsPipeline>(dxgi, shaderCompiler);
	model3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::Model3D);
	SetPipelineState(GraphicsPipelineStateType::Model3D);

	// SkyBoxのグラフィックスパイプランを生成、初期化
	skyBoxGraphicsPipeline_ = std::make_unique<SkyBoxGraphicsPipeline>(dxgi, shaderCompiler);
	skyBoxGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::SkyBox);
	SetPipelineState(GraphicsPipelineStateType::SkyBox);

	// パーティクルエフェクトのグラフィックスパイプラインを生成、初期化
	particleEffect3DGraphicsPipeline_ = std::make_unique<ParticleEffectGraphicsPipeline>(dxgi, shaderCompiler);
	particleEffect3DGraphicsPipeline_->Initialize();
	SetRootSignature(GraphicsPipelineStateType::ParticleEffect3D);
	SetPipelineState(GraphicsPipelineStateType::ParticleEffect3D);


	// 他のパイプラインステートが追加された場合はここに追加
}

ID3D12RootSignature* GraphicsPipelineManager::GetRootSignature(GraphicsPipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* GraphicsPipelineManager::GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][static_cast<uint32_t>(blendMode)].Get();
}

void GraphicsPipelineManager::SetRootSignature(GraphicsPipelineStateType pipelineState) {
	// パイプラインステートごとに対応するルートシグネチャを設定
	switch (pipelineState) {
	case GraphicsPipelineStateType::Sprite:
		// スプライト描画用のルートシグネチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = spriteGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Line3D:
		// 3Dライン描画用のルートシグネチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = line3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Triangle3D:
		// 3D三角形描画用のルートシグネチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = triangle3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Plane3D:
		// 3D板ポリ用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = plane3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Box3D:
		// 3Dボックス用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = box3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Sphere3D:
		// 3D球体用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = sphere3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Ring3D:
		// 3Dリング用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = ring3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Cylinder3D:
		// 3Dシリンダー用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = cylinder3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::Model3D:
		// 3Dモデル用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = model3DGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::SkyBox:
		// スカイボックス用のルートシグネイチャを設定
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = skyBoxGraphicsPipeline_->GetRootSignature();
		break;
	case GraphicsPipelineStateType::ParticleEffect3D:
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = particleEffect3DGraphicsPipeline_->GetRootSignature();
		break;
		// 他のパイプラインステートが追加された場合はここに追加
	}
}

void GraphicsPipelineManager::SetPipelineState(GraphicsPipelineStateType pipelineState) {
	switch (pipelineState) {
	case GraphicsPipelineStateType::Sprite:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = spriteGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Line3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = line3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Triangle3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = triangle3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Plane3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = plane3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Box3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = box3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Sphere3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = sphere3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Ring3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = ring3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Cylinder3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = cylinder3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::Model3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = model3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::SkyBox:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = skyBoxGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case GraphicsPipelineStateType::ParticleEffect3D:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = particleEffect3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;

		// 他のパイプラインステートが追加された場合はここに追加
	}
}
