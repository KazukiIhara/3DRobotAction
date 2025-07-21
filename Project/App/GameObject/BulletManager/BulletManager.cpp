#include "BulletManager.h"

#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"

BulletManager::BulletManager(AttackCollisionManager* attackColliderManager) {
	bullets_.clear();
	MAGIAssert::Assert(attackColliderManager, "AttackColliderManager is Null");
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

void BulletManager::AddBullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos, int32_t damage) {
	// コライダーを作成
	std::shared_ptr<AttackCollider> bulletCollider = std::make_unique<AttackCollider>(tag, wPos, Vector3(-0.4f, -0.4f, -0.4f), Vector3(0.4f, 0.4f, 0.4f), damage);
	// コリジョンマネージャに追加
	std::weak_ptr<AttackCollider> temp = atkColliderManager_->AddAttackCollider(std::move(bulletCollider));

	Bullet newBullet = Bullet(dir, speed, wPos, temp);
	bullets_.push_back(newBullet);
}