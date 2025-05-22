#include "GameObject3DManager.h"

#include "Logger/Logger.h"

#include <cassert>

GameObject3DManager::GameObject3DManager() {
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObject3DManager::~GameObject3DManager() {
	Logger::Log("GameObject3DManager Finalize\n");
}

void GameObject3DManager::Update() {
	for (auto& gameObject : gameObjects3D_) {
		// ポインタが有効かつ親がいない場合更新
		if (gameObject.second && !gameObject.second->GetParent()) {
			// オブジェクトの子を更新
			gameObject.second->UpdateHierarchy();
			// ワールド行列の更新書影
			gameObject.second->UpdateWorldTransformHierarchy();
		}
	}
}

void GameObject3DManager::Draw() {
	for (auto& gameObject : gameObjects3D_) {
		if (gameObject.second) {
			// 描画処理
			gameObject.second->Draw();
		}
	}
}

std::string GameObject3DManager::Add(std::unique_ptr<GameObject3D> newGameObject3D) {
	// 新しいオブジェクト名を決定
	std::string uniqueName = newGameObject3D->name;
	int suffix = 1;

	// 同じ名前が既に存在する場合、ユニークな名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(gameObjects3D_.begin(), gameObjects3D_.end(), [&](const auto& gameObject) {
			return gameObject.second->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = newGameObject3D->name + "_" + std::to_string(suffix);
		suffix++;
	}

	// ユニークな名前に改名
	newGameObject3D->name = uniqueName;

	// コンテナに登録
	gameObjects3D_.insert(std::pair(uniqueName, std::move(newGameObject3D)));

	// ユニークな名前を返す
	return uniqueName;
}

GameObject3D* GameObject3DManager::Find(const std::string& objectName) {
	// 指定した名前のオブジェクトを検索
	auto it = gameObjects3D_.find(objectName);

	// 見つかった場合はポインタを返す
	if (it != gameObjects3D_.end()) {
		return it->second.get();
	}

	// 見つからなかった場合は nullptr を返す
	return nullptr;
}

void GameObject3DManager::DeleteGarbages() {
	for (auto it = gameObjects3D_.begin(); it != gameObjects3D_.end(); ) {
		// isAlive が false なら要素を削除
		if (!it->second->isAlive) {
			// 付随するコンポーネントの削除フラグを立てる関数を呼ぶ
			it->second->DeleteComponents();

			// 親子関係の解消

			// 子がいる場合
			if (!it->second->GetChildren()->empty()) {
				// すべての子から自身を削除
				for (auto& child : *it->second->GetChildren()) {
					it->second->DetachChild(child);
				}
			}

			// 親がいる場合
			if (auto* parent = it->second->GetParent()) {
				parent->DetachChild(it->second.get());
			}

			// 削除
			it = gameObjects3D_.erase(it);
		} else {
			++it;
		}
	}
}

void GameObject3DManager::Clear() {
	gameObjects3D_.clear();
}
