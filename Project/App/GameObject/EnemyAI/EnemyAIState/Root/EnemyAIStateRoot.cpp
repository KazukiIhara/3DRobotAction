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

	// 通常の銃ならひとまず撃ちまくる
	MechHandWeapon::Type lefthandWType = mechCore->GetLeftHandWeapon()->GetParam().type;
	if (lefthandWType == MechHandWeapon::Type::Gun) {
		enemyAI->LeftHandWeapon();
	}

	MechHandWeapon::Type righthandWType = mechCore->GetRightHandWeapon()->GetParam().type;
	if (righthandWType == MechHandWeapon::Type::Gun) {
		enemyAI->RightHandWeapon();
	}

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
	Avoid(enemyAI);


	// ロックオン対象がいない場合は索敵ステートに遷移
	if (!mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		enemyAI->ChangeState(EnemyAIState::Search);
		return;
	}

}

void EnemyAIStateRoot::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}

void EnemyAIStateRoot::Avoid(EnemyAI* enemyAI) {
	// 回避処理
	// 回避クールタイムの処理
	if (avoidCoolTimer_ >= 0.0f) {
		avoidCoolTimer_ -= MAGISYSTEM::GetDeltaTime();
	} else {

		// 回避用コライダー取得
		const AvoidCollider avoidCollider = enemyAI->GetAvoidCollider();

		// FOV ラジアン変換
		const float fovYRad = avoidCollider.fovY * (std::numbers::pi_v<float> / 180.0f);
		const float aspect = 16.0f / 9.0f;

		// View / Projection / VP 行列
		const Matrix4x4 V = MakeLookAtMatrix(avoidCollider.eye, avoidCollider.target);
		const Matrix4x4 P = MakePerspectiveFovMatrix(
			fovYRad,
			aspect,
			avoidCollider.nearClip,
			avoidCollider.farClip
		);
		const Matrix4x4 VP = V * P;

		const float radiusSq = avoidCollider.radius * avoidCollider.radius;

		// ミサイルのリストを取得

		// ミサイルの回避処理


		// 弾のリストを取得
		const std::vector<Bullet> bullets_ = enemyAI->GetAttackObjectManager()->GetBullets();

		// 弾の回避処理
		if (!bullets_.empty()) {
			for (auto bullet : bullets_) {
				AttackCollider* collider = bullet.GetAttackCollider();
				// 自軍の弾だったら処理しない
				if (collider->GetFriendlyTag() == FriendlyTag::EnemySide) {
					continue;
				}

				// 弾の座標を取得
				const Vector3 bulletPos = bullet.GetWorldPos();

				// 周辺探索半径に入っていたら遷移
				{
					const Vector3 diff = bulletPos - avoidCollider.wPos;
					if (LengthSquared(diff) <= radiusSq) {
						avoidCoolTimer_ = Random::GenerateFloat(0.0f, 2.0f);
						enemyAI->ChangeState(EnemyAIState::Avoid);
						return;
					}
				}

				// 視錐台判定
				Vector4 posH{ bulletPos.x,bulletPos.y,bulletPos.z,1.0f };
				Vector4 clip = Transform(posH, VP);

				// 背面判定
				if (clip.w <= 0.0f) {
					continue;
				}

				Vector3 ndc{ clip.x, clip.y, clip.z };
				ndc /= clip.w;

				// NDC 範囲外 → 視錐台外
				if (std::abs(ndc.x) > 1.0f ||
					std::abs(ndc.y) > 1.0f ||
					ndc.z < 0.0f || ndc.z > 1.0f) {
					continue;
				}

				// ここまで来たら視錐台内に入っているので遷移
				avoidCoolTimer_ = Random::GenerateFloat(0.0f, 2.0f);
				enemyAI->ChangeState(EnemyAIState::Avoid);
				return;

			}
		}



	}
}
