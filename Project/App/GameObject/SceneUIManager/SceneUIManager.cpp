#include "SceneUIManager.h"
#include "MAGI.h"

SceneUIManager::SceneUIManager(const std::string& sceneName) {
	sceneName_ = sceneName;
	uiDataPath_ = sceneName_ + "UIData";

	// UIパラメータのグループを作成
	MAGISYSTEM::AddParameterGroup(uiDataPath_);

}

void SceneUIManager::Add(std::unique_ptr<GameUI2D> ui) {

}

void SceneUIManager::Clear() {
	sceneUIList_.clear();
	sceneUIMap_.clear();
}