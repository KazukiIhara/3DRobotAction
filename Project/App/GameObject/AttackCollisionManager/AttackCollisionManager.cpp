#include "AttackCollisionManager.h"

AttackCollisionManager::AttackCollisionManager() {
	mechs_.clear();
	attackColliders_.clear();
}

void AttackCollisionManager::Update() {
	for (auto& collider : attackColliders_) {
		collider->Update();
	}
}

void AttackCollisionManager::Draw() {
	for (auto& collider : attackColliders_) {
		collider->Draw();
	}
}

void AttackCollisionManager::AddMech(std::weak_ptr<MechCore> mech) {
	mechs_.push_back(mech);
}

void AttackCollisionManager::AddAttackCollider(std::shared_ptr<AttackCollider> collider) {
	attackColliders_.push_back(collider);
}