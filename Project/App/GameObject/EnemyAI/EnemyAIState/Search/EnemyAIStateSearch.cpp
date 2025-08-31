#include "EnemyAIStateSearch.h"

#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void EnemyAIStateSearch::Enter([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}

void EnemyAIStateSearch::Update([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	// 自機の座標を取得
	const Vector3 pos = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();
	// プレイヤーの機体のポインタを取得
	MechCore* playerMech = enemyAI->GetPlayerMech();
	// プレイヤーの座標を取得
	Vector3 playerPos = playerMech->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();

	// 方向を取得
	Vector3 dir = pos - playerPos;

	enemyAI->MoveDir(Vector2(dir.x, dir.z));

	// 自機がロックオン対象になったら通常行動に入る
	if (mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		enemyAI->ChangeState(EnemyAIState::Root);
	}

}

void EnemyAIStateSearch::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
