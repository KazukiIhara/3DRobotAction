#include "EnemyAIStateRoot.h"

#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void EnemyAIStateRoot::Enter([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	jumpTimer_ = jumpTime_;
}

void EnemyAIStateRoot::Update([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
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
		targetMoveDir_.y = 1.0f;
	} else {
		targetMoveDir_.y = 0.0f;
	}

	// 移動方向を徐々に補間
	currentMoveDir_ = Lerp(currentMoveDir_, targetMoveDir_, 0.01f);

	// 旋回行動
	enemyAI->MoveDir(currentMoveDir_);


	// ロックオン対象がいない場合は索敵ステートに遷移
	if (!mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		enemyAI->ChangeState(EnemyAIState::Search);
	}

}

void EnemyAIStateRoot::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
