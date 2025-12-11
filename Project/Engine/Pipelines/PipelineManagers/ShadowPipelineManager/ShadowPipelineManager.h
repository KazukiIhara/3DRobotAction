#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "Enums/ShadowPipelineEnum.h"
#include "DirectX/ComPtr/ComPtr.h"

// Triangle
// Plane
#include "ShadowPipelines/BoxShadowPipeline/BoxShadowPipeline.h"
#include "ShadowPipelines/SphereShadowPipeline/SphereShadowPipeline.h"
// Ring
#include "ShadowPipelines/CylinderShadowPipeline/CylinderShadowPipeline.h"
#include "ShadowPipelines/ModelShadowPipeline/ModelShadowPipeline.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// 影描画パイプラインマネージャ
/// </summary>
class ShadowPipelineManager {
public:
	ShadowPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~ShadowPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(ShadowPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(ShadowPipelineStateType pipelineState);

	// ルートシグネイチャをセット
	void SetRootSignature(ShadowPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(ShadowPipelineStateType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kShadowPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> shadowPipelineStates_[kShadowPipelineStateNum];
private:
	// ボックス用のシャドウパイプライン
	std::unique_ptr<BoxShadowPipeline> boxShadowPipeline_ = nullptr;
	// 球体用のシャドウパイプライン
	std::unique_ptr<SphereShadowPipeline> sphereShadowPipeline_ = nullptr;
	// シリンダー用のシャドウパイプライン
	std::unique_ptr<CylinderShadowPipeline> cylinderShadowPipeline_ = nullptr;
	// モデル用のシャドウパイプライン
	std::unique_ptr<ModelShadowPipeline> modelShadowPipeline_ = nullptr;

};