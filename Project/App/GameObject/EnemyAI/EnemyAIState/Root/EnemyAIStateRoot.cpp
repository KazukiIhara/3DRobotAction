#include "EnemyAIStateRoot.h"

#include "Random/Random.h"
#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Math/Utility/MathUtility.h"

#include "GameObject/AttackObjectManager/AttackObjectManager.h"

using namespace MAGIMath;

void EnemyAIStateRoot::Enter([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	//
	// 各変数リセット
	//
	jumpTimer_ = Random::GenerateFloat(0.0f, 1.0f);
	boostTimer_ = Random::GenerateFloat(0.2f, 1.0f);
	avoidCoolTimer_ = Random::GenerateFloat(0.0f, 2.0f);

	// 旋回方向に応じて旋回
	if (enemyAI->GetRootDir() == RootDir::Left) {
		targetMoveDir_.x = -1.0f;
		currentMoveDir_.x = -1.0f;
	} else {
		targetMoveDir_.x = 1.0f;
		currentMoveDir_.x = 1.0f;
	}
}

void EnemyAIStateRoot::Update([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	// 攻撃の処理
	enemyAI->LeftHandWeapon();
	enemyAI->RightHandWeapon();

	// ジャンプの処理

	// タイマーが0になったら飛ぶ
	if (jumpTimer_ >= 0.0f) {
		// ジャンプのタイマーを減らす
		jumpTimer_ -= MAGISYSTEM::GetDeltaTime();
	} else {
		// ブーストタイマーが0より大きければジャンプ
		if (boostTimer_ > 0.0f) {
			boostTimer_ -= MAGISYSTEM::GetDeltaTime();
			enemyAI->Jump();
		} else {
			jumpTimer_ = Random::GenerateFloat(0.2f, 2.0f);
			boostTimer_ = Random::GenerateFloat(0.5f, 1.0f);
		}
	}

	// 自機の座標を取得
	const Vector3 pos = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();

	// プレイヤーの機体のポインタを取得
	MechCore* playerMech = enemyAI->GetPlayerMech();
	// プレイヤーの座標を取得
	const Vector3 playerPos = playerMech->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();

	// 距離を図る
	const float distance = Length(pos - playerPos);

	// 旋回方向に応じて旋回
	if (enemyAI->GetRootDir() == RootDir::Left) {
		targetMoveDir_.x = -1.0f;
	} else {
		targetMoveDir_.x = 1.0f;
	}

	// 距離によって移動の向きを変更
	if (distance > targetRange_) {
		targetMoveDir_.y = 2.0f;
	} else {
		targetMoveDir_.y = 0.0f;
	}

	// 移動方向を徐々に補間
	currentMoveDir_ = Lerp(currentMoveDir_, targetMoveDir_, 0.01f);

	// 旋回行動
	enemyAI->MoveDir(currentMoveDir_);

	// 回避処理
	// 回避クールタイムの処理
	if (avoidCoolTimer_ >= 0.0f) {
		avoidCoolTimer_ -= MAGISYSTEM::GetDeltaTime();
	} else {
		// 弾のリストを取得
		const std::vector<Bullet> bullets_ = enemyAI->GetBulletManager()->GetBullets();

		if (!bullets_.empty()) {
			// 回避用コライダー取得
			const Vector3 aMin = enemyAI->GetAvoidCollider().min;
			const Vector3 aMax = enemyAI->GetAvoidCollider().max;

			for (auto bullet : bullets_) {
				AttackCollider* collider = bullet.GetAttackCollider();
				// 自軍の弾だったら処理しない
				if (collider->GetFriendlyTag() == FriendlyTag::EnemySide) {
					continue;
				}
				const Vector3 bulletPos = bullet.GetWorldPos();

				// 弾が回避用コライダーの中にあったら回避行動
				if (bulletPos.x >= aMin.x && bulletPos.x <= aMax.x &&
					bulletPos.y >= aMin.y && bulletPos.y <= aMax.y &&
					bulletPos.z >= aMin.z && bulletPos.z <= aMax.z) {
					avoidCoolTimer_ = Random::GenerateFloat(0.0f, 2.0f);
					// 回避ステートに遷移
					enemyAI->ChangeState(EnemyAIState::Avoid);
					return;
				}

			}
		}

	}


	// ロックオン対象がいない場合は索敵ステートに遷移
	if (!mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		enemyAI->ChangeState(EnemyAIState::Search);
		return;
	}

}

void EnemyAIStateRoot::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
