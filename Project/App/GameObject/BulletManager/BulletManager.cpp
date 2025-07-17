#include "BulletManager.h"

#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"

BulletManager::BulletManager(AttackCollisionManager* attackColliderManager) {
	bullets_.clear();
	MAGIAssert::Assert(attackColliderManager,"AttackColliderManager is Null");
	atkColliderManager_ = attackColliderManager;
}

void BulletManager::Update() {
	for (auto& bullet : bullets_) {
		bullet.Update();
	}

	// 死亡フラグの立っている弾をまとめて除去
	bullets_.erase(
		std::remove_if(bullets_.begin(), bullets_.end(),
			[](const Bullet& b) { return !b.GetIsAlive(); }),
		bullets_.end());
}

void BulletManager::Draw() {
	for (auto& bullet : bullets_) {
		bullet.Draw();
	}
}

void BulletManager::AddBullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos) {
	Bullet newBullet = Bullet(tag, dir, speed, wPos);
	bullets_.push_back(newBullet);
}