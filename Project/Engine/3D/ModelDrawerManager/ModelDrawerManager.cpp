#include "ModelDrawerManager.h"

// MyHedder
#include "MAGIAssert/MAGIAssert.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "PipelineManagers/ShadowPipelineManager/ShadowPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"

ModelDrawerManager::ModelDrawerManager(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	ShadowPipelineManager* shadowPipelineManager,
	Camera3DManager* camera3DManager
) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetShadowPipelineManager(shadowPipelineManager);
	SetCamera3DManager(camera3DManager);

	Logger::Log("ModelDrawerManager Initialize\n");
}

ModelDrawerManager::~ModelDrawerManager() {
	Logger::Log("ModelDrawerManager Initialize\n");
}

void ModelDrawerManager::CreateModelDrawer(const std::string& modelDrawerName, const ModelData& modelData) {
	// 追加する描画クラスを作成
	std::unique_ptr<ModelDrawer> newModelDrawer = std::make_unique<ModelDrawer>(modelData);
	// ペアを作って挿入
	modelDrawers_.insert(std::make_pair(modelDrawerName, std::move(newModelDrawer)));
}

void ModelDrawerManager::CreateSkinModelDrawer(const std::string& modelDrawerName, const ModelData& modelData) {
	// 追加する描画クラスを作成
	std::unique_ptr<SkinModelDrawer> newModelDrawer = std::make_unique<SkinModelDrawer>(modelData);
	// ペアを作って挿入
	skinModelDrawers_.insert(std::make_pair(modelDrawerName, std::move(newModelDrawer)));
}

void ModelDrawerManager::DrawModel(const std::string& modelDrawerName, const Matrix4x4& worldMatrix, const ModelMaterial& material) {
	auto it = modelDrawers_.find(modelDrawerName);
	if (it != modelDrawers_.end()) {
		it->second->AddDrawCommand(worldMatrix, material);
		return;
	}

#if defined(DEBUG)||defined(DEVELOP)
	MAGIAssert::Assert(false, "ModelDrawer" + modelDrawerName + "が見つかりませんでした。");
	return;
#endif
}

void ModelDrawerManager::DrawSkinModel(const std::string& modelDrawerName, const Matrix4x4& worldMatrix, const ModelMaterial& material) {
	auto it = skinModelDrawers_.find(modelDrawerName);
	if (it != skinModelDrawers_.end()) {
		it->second->AddDrawCommand(worldMatrix, material);
		return;
	}

#if defined(DEBUG)||defined(DEVELOP)
	MAGIAssert::Assert(false, "SkinModelDrawer" + modelDrawerName + "が見つかりませんでした。");
	return;
#endif
}

void ModelDrawerManager::UpdateAll() {
	// 全モデル描画クラスを更新
	for (auto& modelDrawer : modelDrawers_) {
		modelDrawer.second->Update();
	}

	for (auto& skinModelDraewr : skinModelDrawers_) {
		skinModelDraewr.second->Update();
	}
}

void ModelDrawerManager::DrawAll(BlendMode mode) {
	// ルーシグネイチャをセット
	directXCommand_->GetList()->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Model3D));

	// 描画
	for (auto& modelDrawer : modelDrawers_) {
		modelDrawer.second->Draw(mode);
	}

	for (auto& skinModelDrawer : skinModelDrawers_) {
		skinModelDrawer.second->Draw(mode);
	}
}

void ModelDrawerManager::DrawShadowAll(BlendMode mode) {
	// ルーシグネイチャをセット
	directXCommand_->GetList()->SetGraphicsRootSignature(shadowPipelineManager_->GetRootSignature(ShadowPipelineStateType::Model));

	// 描画
	for (auto& modelDrawer : modelDrawers_) {
		modelDrawer.second->DrawShadow(mode);
	}

	for (auto& skinModelDrawer : skinModelDrawers_) {
		skinModelDrawer.second->DrawShadow(mode);
	}
}

void ModelDrawerManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void ModelDrawerManager::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void ModelDrawerManager::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void ModelDrawerManager::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void ModelDrawerManager::SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager) {
	assert(shadowPipelineManager);
	shadowPipelineManager_ = shadowPipelineManager;
}

void ModelDrawerManager::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}
