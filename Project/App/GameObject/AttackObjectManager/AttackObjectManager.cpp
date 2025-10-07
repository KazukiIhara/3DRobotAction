#include "AttackObjectManager.h"

#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"

AttackObjectManager::AttackObjectManager(AttackCollisionManager* attackColliderManager) {
	bullets_.clear();
	MAGIAssert::Assert(attackColliderManager, "AttackColliderManager is Null");
	atkColliderManager_ = attackColliderManager;
}

void AttackObjectManager::Update() {
	// 弾
	for (auto& bullet : bullets_) {
		bullet.Update();
	}

	// 死亡フラグの立っている弾をまとめて除去
	bullets_.erase(
		std::remove_if(bullets_.begin(), bullets_.end(),
			[](const Bullet& b) { return !b.GetIsAlive(); }),
		bullets_.end());

	// ミサイル
	for (auto& missile : missiles_) {
		missile.Update();
	}

	// 死亡フラグの立っているミサイルをまとめて除去
	missiles_.erase(
		std::remove_if(missiles_.begin(), missiles_.end(),
			[](const Missile& m) { return !m.GetIsAlive(); }),
		missiles_.end());
}

void AttackObjectManager::Draw() {
	// 弾
	for (auto& bullet : bullets_) {
		bullet.Draw();
	}
}

const std::vector<Bullet>& AttackObjectManager::GetBullets() {
	return bullets_;
}

void AttackObjectManager::AddBullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos, int32_t damage) {
	// コライダーを作成
	std::shared_ptr<AttackCollider> bulletCollider = std::make_unique<AttackCollider>(tag, AttackType::Bullet, wPos, Vector3(-0.2f, -0.2f, -0.2f), Vector3(0.2f, 0.2f, 0.2f), damage);
	// コリジョンマネージャに追加
	std::weak_ptr<AttackCollider> temp = atkColliderManager_->AddAttackCollider(std::move(bulletCollider));

	// 弾を作成
	Bullet newBullet = Bullet(dir, speed, wPos, temp);
	bullets_.push_back(newBullet);
}

void AttackObjectManager::AddMissile(const FriendlyTag& tag, const MissileType& missileType, const Vector3& wPos, float speed, float acc, float maxSpeed, const Vector3& dir, int32_t damage, std::weak_ptr<MechCore> target) {
	// コライダーを作成
	std::shared_ptr<AttackCollider> missileCollider = std::make_unique<AttackCollider>(tag, AttackType::Missile, wPos, Vector3(-0.4f, -0.4f, -0.4f), Vector3(0.4f, 0.4f, 0.4f), damage);
	// コリジョンマネージャに追加
	std::weak_ptr<AttackCollider> temp = atkColliderManager_->AddAttackCollider(std::move(missileCollider));

	// ミサイルを作成
	Missile newMissile = Missile(missileType, wPos, speed, acc, maxSpeed, dir, target, temp);
	missiles_.push_back(newMissile);
}