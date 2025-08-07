#include "BaseScene.h"

#include "SceneManager/SceneManager.h"

BaseScene::BaseScene(SceneManager* sceneManager) {
	sceneManager_ = sceneManager;
}

void BaseScene::ChangeScene(const std::string& sceneName) {
	sceneManager_->ChangeScene(sceneName);
}

GameData& BaseScene::GetData()const {
	return sceneManager_->GetData();
}