#include "GameObject3DGroupManager.h"

#include "Logger/Logger.h"

#include <cassert>

GameObject3DGroupManager::GameObject3DGroupManager() {
	Logger::Log("GameObject3DGroupManager Initialize\n");
}

GameObject3DGroupManager::~GameObject3DGroupManager() {
	Logger::Log("GameObject3DGroupManager Finalize\n");
}

void GameObject3DGroupManager::Update() {
	for (auto& gameObject3DGroup : gameObject3DGroups_) {
		if (gameObject3DGroup.second) {
			gameObject3DGroup.second->UpdateData();
		}
	}
}

void GameObject3DGroupManager::Draw() {
	for (auto& gameObject3DGroup : gameObject3DGroups_) {
		if (gameObject3DGroup.second) {
			gameObject3DGroup.second->Draw();
		}
	}
}

std::string GameObject3DGroupManager::Add(std::unique_ptr<GameObject3DGroup> newGameObjectGroup) {
	// 新しいオブジェクト名を決定
	std::string uniqueName = newGameObjectGroup->GetName();
	int suffix = 1;

	// 同じ名前が既に存在する場合、ユニークな名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(gameObject3DGroups_.begin(), gameObject3DGroups_.end(), [&](const auto& gameObject) {
			return gameObject.second->GetName() == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = newGameObjectGroup->GetName() + "_" + std::to_string(suffix);
		suffix++;
	}

	// ユニークな名前に改名
	newGameObjectGroup->GetName() = uniqueName;

	// コンテナに登録
	gameObject3DGroups_.insert(std::pair(uniqueName, std::move(newGameObjectGroup)));

	// ユニークな名前を返す
	return uniqueName;

}

GameObject3DGroup* GameObject3DGroupManager::Find(const std::string& objectGroupName) {
	// 指定した名前のグループを検索
	auto it = gameObject3DGroups_.find(objectGroupName);

	// 見つかった場合はポインタを返す
	if (it != gameObject3DGroups_.end()) {
		return it->second.get();
	}

	// 見つからなかった場合は nullptr を返す
	return nullptr;
}

void GameObject3DGroupManager::DeleteGarbages() {

}

void GameObject3DGroupManager::Clear() {
	gameObject3DGroups_.clear();
}
