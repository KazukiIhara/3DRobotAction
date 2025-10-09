#include "EnemyAIStateSearch.h"

#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/PlayerCamera/PlayerCamera.h"
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
	const Vector3 dir = Normalize(playerPos - pos);

	// クオータニオンをセット
	if (auto mainCam = dynamic_cast<MechCamera*>(mechCore->GetGameObject().lock()->GetCamera3D("MainCamera").lock().get())) {
		mainCam->SetCameraQuaternion(DirectionToQuaternion(dir));
	}

	// 正面に移動
	enemyAI->MoveDir(Vector2(0.0f, 1.0f));

	// 自機がロックオン対象になったら通常行動に入る
	if (mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		enemyAI->ChangeState(EnemyAIState::Root);
		return;
	}

}

void EnemyAIStateSearch::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
