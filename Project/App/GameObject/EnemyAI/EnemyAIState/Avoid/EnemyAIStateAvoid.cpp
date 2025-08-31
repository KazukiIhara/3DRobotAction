#include "EnemyAIStateAvoid.h"
#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void EnemyAIStateAvoid::Enter([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	// 現在の旋回方向と逆側にQB
	if (enemyAI->GetRootDir() == RootDir::Left) {
		enemyAI->MoveDir(Vector2(1.0f, 0.0f));
		enemyAI->SetRootDir(RootDir::Right);
	} else {
		enemyAI->MoveDir(Vector2(-1.0f, 0.0f));
		enemyAI->SetRootDir(RootDir::Left);
	}
	// クイックブーストする
	enemyAI->QuickBoost();
}

void EnemyAIStateAvoid::Update([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	// 現在のステートを取得
	MechCoreState currentState = mechCore->GetCurrentState();
	// QBが終了していたら通常状態に遷移
	if (currentState != MechCoreState::QuickBoost) {
		enemyAI->ChangeState(EnemyAIState::Root);
	}

}

void EnemyAIStateAvoid::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}