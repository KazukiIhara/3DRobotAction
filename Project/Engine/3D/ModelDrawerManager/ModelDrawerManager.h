#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>

// MyHedder
#include "Structs/ModelStruct.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"
#include "3D/Drawer3D/ModelDrawer/ModelDrawer.h"
#include "3D/Drawer3D/SkinModelDrawer/SkinModelDrawer.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class ShadowPipelineManager;
class Camera3DManager;

/// <summary>
/// モデル描画クラスのマネージャー
/// </summary>
class ModelDrawerManager {
public:
	ModelDrawerManager(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		ShadowPipelineManager* shadowPipelineManager,
		Camera3DManager* camera3DManager
	);
	~ModelDrawerManager();

	void CreateModelDrawer(const std::string& modelDrawerName, const ModelData& modelData);
	void CreateSkinModelDrawer(const std::string& modelDrawerName, const ModelData& modelData);

	void DrawModel(const std::string& modelDrawerName, const Matrix4x4& worldMatrix, const ModelMaterial& material);
	void DrawSkinModel(const std::string& modelDrawerName, const Matrix4x4& worldMatrix, const ModelMaterial& material);

	void UpdateAll();
	void DrawAll(BlendMode mode);

	void DrawShadowAll(BlendMode mode);

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);

private:
	// 描画クラスのコンテナ
	std::unordered_map<std::string, std::unique_ptr<ModelDrawer>> modelDrawers_;
	std::unordered_map<std::string, std::unique_ptr<SkinModelDrawer>> skinModelDrawers_;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	ShadowPipelineManager* shadowPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
};