#include "ColliderManager.h"
#include "Logger/Logger.h"

#include <cassert>

ColliderManager::ColliderManager() {
	Clear();
	Logger::Log("ColliderManager Initialize\n");
}

ColliderManager::~ColliderManager() {
	Logger::Log("ColliderManager Finalize\n");
}

void ColliderManager::Update() {
	for (auto& collider : colliders_) {
		if (collider) {
			collider->Update();
			collider->TransferShape();
		}
	}
}

std::string ColliderManager::Create(const std::string& name, Collider3DType colliderType) {
	// 新しいコライダー名を決定
	std::string uniqueName = name;
	int suffix = 1;

	// 同じ名前が既に存在する場合、一意な名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(colliders_.begin(), colliders_.end(), [&](const auto& collider) {
			return collider->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = name + "_" + std::to_string(suffix);
		suffix++;
	}

	// 追加するコライダー
	std::unique_ptr<BaseCollider3D> newCollider;
	switch (colliderType) {
	case Collider3DType::Sphere:
		newCollider = std::make_unique<SphereCollider>(uniqueName, currentID_, colliderType);
		break;
	case Collider3DType::AABB:
		newCollider = std::make_unique<AABBCollider>(uniqueName, currentID_, colliderType);
		break;
	case Collider3DType::OBB:
		break;
	}

	// コンテナに登録
	colliders_.push_back(std::move(newCollider));

	// 識別IDをインクリメント
	currentID_++;

	// 作成した名前を返す
	return uniqueName;
}

void ColliderManager::Remove(const std::string& name) {
	// ベクターを走査して、名前が一致するコライダーを探す
	for (auto it = colliders_.begin(); it != colliders_.end(); ++it) {
		// (*it)->name_ でコライダー名を取得
		if ((*it)->name == name) {
			colliders_.erase(it);
			return;  // 見つかったら削除して関数を抜ける
		}
	}
	// 見つからなかった場合
	assert(false && "Not Found Collider to Remove");
}

BaseCollider3D* ColliderManager::Find(const std::string& name) {
	// ベクターを走査して、名前が一致するコライダーを探す
	for (auto& collider : colliders_) {
		if (collider && collider->name == name) {
			return collider.get();  // ポインタを返す
		}
	}

	// 見つからない場合
	assert(false && "Not Found Collider");
	return nullptr;
}

void ColliderManager::Clear() {
	colliders_.clear();
	currentID_ = 0;
}

void ColliderManager::DeleteGarbages() {
	colliders_.erase(
		std::remove_if(
			colliders_.begin(),
			colliders_.end(),
			[](const std::unique_ptr<BaseCollider3D>& collider) {
				return !collider->isAlive;
			}
		),
		colliders_.end()
	);
}

const std::vector<std::unique_ptr<BaseCollider3D>>& ColliderManager::GetColliders() const {
	return colliders_;
}