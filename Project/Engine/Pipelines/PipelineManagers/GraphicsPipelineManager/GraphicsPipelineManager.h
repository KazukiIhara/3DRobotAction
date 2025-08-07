#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "Enums/GraphicsPipelineEnum.h"

#include "GraphicsPipelines/SpriteGraphicsPipeline/SpriteGraphicsPipeline.h"

#include "GraphicsPipelines/Line3DGraphicsPipeline/Line3DGraphicsPipeline.h"
#include "GraphicsPipelines/Triangle3DGraphicsPipeline/Triangle3DGraphicsPipeline.h"
#include "GraphicsPipelines/Plane3DGraphicsPipeline/Plane3DGraphicsPipeline.h"
#include "GraphicsPipelines/Box3DGraphicsPipeline/Box3DGraphicsPipeline.h"
#include "GraphicsPipelines/Sphere3DGraphicsPipeline/Sphere3DGraphicsPipeline.h"
#include "GraphicsPipelines/Ring3DGraphicsPipeline/Ring3DGraphicsPipeline.h"
#include "GraphicsPipelines/Cylinder3DGraphicsPipeline/Cylinder3DGraphicsPipeline.h"

#include "GraphicsPipelines/Model3DGraphicsPipeline/Model3DGraphicsPipeline.h"

#include "GraphicsPipelines/SkyBoxGraphicsPipeline/SkyBoxGraphicsPipeline.h"


#include "GraphicsPipelines/ParticleEffect3DGraphicsPipeline/ParticleEffect3DGraphicsPipeline.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// 描画パイプラインマネージャ
/// </summary>
class GraphicsPipelineManager {
public:
	GraphicsPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~GraphicsPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(GraphicsPipelineStateType pipelineState);

private: // メンバ変数
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kGraphicsPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> graphicsPipelineStates_[kGraphicsPipelineStateNum][kBlendModeNum];

private:

	// SpriteGraphicsPipeline
	std::unique_ptr<SpriteGraphicsPipeline> spriteGraphicsPipeline_ = nullptr;

	// Line3DGrahicsPipeline
	std::unique_ptr<Line3DGraphicsPipeline> line3DGraphicsPipeline_ = nullptr;
	// Triangle3DGraphicsPipeline
	std::unique_ptr<Triangle3DGraphicsPipeline> triangle3DGraphicsPipeline_ = nullptr;
	// Plane3DGraphicsPipeline
	std::unique_ptr<Plane3DGraphicsPipeline> plane3DGraphicsPipeline_ = nullptr;
	// Box3DGraphicsPipeline
	std::unique_ptr<Box3DGraphicsPipeline> box3DGraphicsPipeline_ = nullptr;
	// Sphere3DGraphicsPipeline
	std::unique_ptr<Sphere3DGraphicsPipeline> sphere3DGraphicsPipeline_ = nullptr;
	// Ring3DGraphicsPipeline
	std::unique_ptr<Ring3DGraphicsPipeline> ring3DGraphicsPipeline_ = nullptr;
	// Cylinder3DGraphicsPipeline
	std::unique_ptr<Cylinder3DGraphicsPipeline> cylinder3DGraphicsPipeline_ = nullptr;

	// Model3DGraphicsPipeline
	std::unique_ptr<Model3DGraphicsPipeline> model3DGraphicsPipeline_ = nullptr;

	// SkyBoxGraphicsPipeline
	std::unique_ptr<SkyBoxGraphicsPipeline> skyBoxGraphicsPipeline_ = nullptr;

	// ParticleEffect3DGraphicsPipeline
	std::unique_ptr<ParticleEffectGraphicsPipeline> particleEffect3DGraphicsPipeline_ = nullptr;
};