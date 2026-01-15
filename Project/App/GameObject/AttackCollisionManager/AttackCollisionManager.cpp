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
#if defined(DEBUG) || defined(DEVELOP)
	for (auto& collider : attackColliders_) {
		collider->Draw();
	}
#endif
}

void AttackCollisionManager::CheckCollision() {
	for (auto& mech : mechs_) {
		// 機体のタグを取得
		const FriendlyTag mechTag = mech->GetFriendlyTag();
		if (auto m = mech) {

			// 機体のAABBコライダーを取得
			const Vector3 mMin = m->GetCollider()->GetMinW();
			const Vector3 mMax = m->GetCollider()->GetMaxW();

			// 機体のジャスト回避用の球体コライダー取得
			const Vector3 mWpos = m->GetJustDodgeCollider()->GetWorldPos();
			const float mRadius = m->GetJustDodgeCollider()->GetRadius();

			// 場にある攻撃コライダーの数だけループ
			for (auto& atkCollider : attackColliders_) {
				// 攻撃のタグを取得
				const FriendlyTag atkTag = atkCollider->GetFriendlyTag();

				// 同じタグだった場合はスキップ
				if (mechTag == atkTag) continue;

				// 攻撃の球体コライダーを取得
				const Vector3 aPos = atkCollider->GetWPos();
				const float aRadius = atkCollider->GetRadius();

				// ジャスト回避受付状態なら先にジャスト回避判定を取る
				const bool canJustDodge = m->GetStatusComponent()->GetCanJustDodge();
				if (canJustDodge && m->GetFriendlyTag() == FriendlyTag::PlayerSide) {
					// 衝突判定
					if (IsCollisionSphereToSphere(mWpos, mRadius, aPos, aRadius)) {
						// 衝突している

						// 衝突情報をセット
						m->GetJustDodgeCollider()->SetIsHit(true);
						AttackCollider::HitInfo aInfo = {
							true
						};
						atkCollider->SetHitInfo(aInfo);
					}
				}

				// 衝突判定
				if (IsCollisionAABBToSphere(mMin, mMax, aPos, aRadius)) {
					// 衝突している

					// コリジョンインフォをセット
					MechCollider::HitInfo mInfo = {
						true,
						atkCollider->GetWPos(),
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


void AttackCollisionManager::AddMech(MechCore* mech) {
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

bool AttackCollisionManager::IsCollisionSphereToSphere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB) {
	// 中心間距離の二乗を計算
	const Vector3 d = posA - posB;
	const float distSq = d.x * d.x + d.y * d.y + d.z * d.z;

	// 半径の合計
	const float r = radiusA + radiusB;
	const float rSq = r * r;

	// 二乗同士で判定
	return distSq <= rSq;
}

bool AttackCollisionManager::IsCollisionAABBToSphere(const Vector3& minA, const Vector3& maxA, const Vector3& posB, float radiusB) {
	// 球の中心を AABB 内にクランプして最も近い点を求める
	Vector3 closest;
	closest.x = std::clamp(posB.x, minA.x, maxA.x);
	closest.y = std::clamp(posB.y, minA.y, maxA.y);
	closest.z = std::clamp(posB.z, minA.z, maxA.z);

	// 球中心との距離の二乗
	Vector3 diff = posB - closest;
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

	// 半径の二乗と比較
	float rSq = radiusB * radiusB;

	return distSq <= rSq;
}
