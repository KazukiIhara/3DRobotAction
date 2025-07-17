#include "AttackCollisionManager.h"

AttackCollisionManager::AttackCollisionManager() {
	mechs_.clear();
	attackColliders_.clear();
}

void AttackCollisionManager::Update() {
	for (auto& collider : attackColliders_) {
		collider->Update();
	}

	// 死亡フラグの立っている弾をまとめて除去
	attackColliders_.erase(
		std::remove_if(attackColliders_.begin(), attackColliders_.end(),
			[](auto& b) { return !b->GetIsAlive(); }),
		attackColliders_.end());
}

void AttackCollisionManager::Draw() {
	for (auto& collider : attackColliders_) {
		collider->Draw();
	}
}

void AttackCollisionManager::AddMech(std::weak_ptr<MechCore> mech) {
	mechs_.push_back(mech);
}

std::weak_ptr<AttackCollider> AttackCollisionManager::AddAttackCollider(std::shared_ptr<AttackCollider> collider) {
	attackColliders_.push_back(std::move(collider));
	return attackColliders_.back();
}