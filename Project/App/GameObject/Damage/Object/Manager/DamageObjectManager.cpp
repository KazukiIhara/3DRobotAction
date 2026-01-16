#include "DamageObjectManager.h"

DamageObjectManager::DamageObjectManager(DamageCollisionSystem* collisionSystem) {
	objects_.clear();
	collisionSystem_ = collisionSystem;
}

void DamageObjectManager::Update() {
	// オブジェクト更新
	for (auto& obj : objects_) {
		if (obj) {
			obj->Update();
		}
	}
	// 死亡フラグの立っているオブジェクト削除
	DeleteGarbage();
}

void DamageObjectManager::Draw() {
	// オブジェクト描画
	for (auto& obj : objects_) {
		if (obj) {
			obj->Draw();
		}
	}
}

void DamageObjectManager::DeleteGarbage() {
	// 削除フラグの立っているオブジェクトをFinalizeを呼んでから削除
	objects_.erase(
		std::remove_if(objects_.begin(), objects_.end(),
			[](const std::unique_ptr<BaseDamageObject>& obj) {
				if (!obj) {
					return true;
				}
				if (obj->GetIsAlive()) {
					return false;
				}
				obj->Finalize();
				return true;
			}),
		objects_.end());
}

void DamageObjectManager::Add(std::unique_ptr<BaseDamageObject> object) {
	if (!object) {
		return;
	}

	object->SetCollisionSystem(collisionSystem_);
	objects_.push_back(std::move(object));
}
