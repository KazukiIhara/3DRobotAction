#include "AttackCollisionManager.h"

#include "GameObject/Mech/MechCore/MechCore.h"

AttackCollisionManager::AttackCollisionManager() {
	mechs_.clear();
	attackColliders_.clear();
}

void AttackCollisionManager::Update() {
	// 死亡フラグの立っているコライダーをまとめて除去
	attackColliders_.erase(
		std::remove_if(attackColliders_.begin(), attackColliders_.end(),
			[](auto& b) { return !b->GetIsAlive(); }),
		attackColliders_.end());

	// コライダーの位置を更新
	for (auto& collider : attackColliders_) {
		collider->Update();
	}

	// 当たり判定を取る(次のフレームに消す用のフラグを立てる)
	CheckCollision();

}

void AttackCollisionManager::Draw() {
	for (auto& collider : attackColliders_) {
		collider->Draw();
	}
}

void AttackCollisionManager::CheckCollision() {
	for (auto& mech : mechs_) {
		if (auto m = mech.lock()) {
			// 機体のAABBコライダーを取得
			const Vector3 mMin = m->GetCollider()->GetMinW();
			const Vector3 mMax = m->GetCollider()->GetMaxW();

			for (auto& atkCollider : attackColliders_) {
				// 攻撃のAABBコライダーを取得
				const Vector3 aMin = atkCollider->GetMinW();
				const Vector3 aMax = atkCollider->GetMaxW();

				// 衝突判定
				if (IsCollisionAABB(mMin, mMax, aMin, aMax)) {
					// コリジョンインフォをセット
					MechCollider::HitInfo mInfo = {
						true,
						atkCollider->GetType(),
						atkCollider->GetDamage(),
					};
					m->GetCollider()->AddHitInfo(mInfo);

					AttackCollider::HitInfo aInfo = {
						true
					};
					atkCollider->SetHitInfo(aInfo);

				}
			}

		}
	}
}

void AttackCollisionManager::AddMech(std::weak_ptr<MechCore> mech) {
	mechs_.push_back(mech);
}

std::weak_ptr<AttackCollider> AttackCollisionManager::AddAttackCollider(std::shared_ptr<AttackCollider> collider) {
	attackColliders_.push_back(std::move(collider));
	return attackColliders_.back();
}

bool AttackCollisionManager::IsCollisionAABB(const Vector3& minA, const Vector3& maxA, const Vector3& minB, const Vector3& maxB) {
	// どれか 1 軸でも離れていたら非交差
	if (maxA.x < minB.x || maxB.x < minA.x) return false;
	if (maxA.y < minB.y || maxB.y < minA.y) return false;
	if (maxA.z < minB.z || maxB.z < minA.z) return false;
	return true;
}
