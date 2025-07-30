#include "GameObject3DManager.h"

#include <cassert>

// MyHedder
#include "Logger/Logger.h" 

GameObject3DManager::GameObject3DManager() {
	Clear();
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObject3DManager::~GameObject3DManager() {
	Clear();
	Logger::Log("GameObject3DManager Finalize\n");
}

std::weak_ptr<GameObject3D> GameObject3DManager::Add(std::shared_ptr<GameObject3D> gameObject3D, bool insertMap) {
	assert(gameObject3D && "GameObject3D must not be null");
	const std::string& name = gameObject3D->GetName();

	// vector に追加
	gameObjects_.push_back(std::move(gameObject3D));
	auto& added = gameObjects_.back();

	if (insertMap) {
		// map に追加
		gameObjectList_.insert_or_assign(name, added);
	}

	// weak_ptr を返す
	return added;
}

std::weak_ptr<GameObject3D> GameObject3DManager::Find(const std::string& objectName) {
	auto it = gameObjectList_.find(objectName);
	if (it == gameObjectList_.end()) {
		// map に登録されていない
		return {};
	}
	// weak_ptr を lock() して生存確認
	if (auto sp = it->second.lock()) {
		return sp;  // 生きている shared_ptr から weak_ptr を作って返す
	} else {
		// expired なら map から削除しておく
		gameObjectList_.erase(it);
		return {};
	}
}

void GameObject3DManager::DeleteGarbage() {
	for (auto& gameobject : gameObjects_) {
		if (gameobject && !gameobject->GetIsAlive()) {
			gameobject->Finalize();
		}
	}

	std::erase_if(gameObjects_, [](const std::shared_ptr<GameObject3D>& gameObject) {
		return gameObject && !gameObject->GetIsAlive();
		});

	std::erase_if(gameObjectList_, [](const std::pair<const std::string, std::weak_ptr<GameObject3D>>& entry) {
		return entry.second.expired();
		});

}

void GameObject3DManager::Clear() {
	for (auto& gameobject : gameObjects_) {
		if (gameobject) {
			gameobject->SetIsAlive(false);
			gameobject->Finalize();
		}
	}
}

void GameObject3DManager::ClearWithOutUnique() {
	for (auto& gameobject : gameObjects_) {
		if (gameobject && !gameobject->GetIsUnique()) {
			gameobject->SetIsAlive(false);
			gameobject->Finalize();
		}
	}
}
