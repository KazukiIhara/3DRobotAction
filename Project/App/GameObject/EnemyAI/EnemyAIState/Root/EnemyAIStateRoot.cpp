#include "EnemyAIStateRoot.h"

#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void EnemyAIStateRoot::Enter(EnemyAI* enemyAI) {
	jumpTimer_ = jumpTime_;
}

void EnemyAIStateRoot::Update(EnemyAI* enemyAI) {
	//// 旋回行動
	//enemyAI->MoveDir(Vector2(1.0f, 1.0f));

	//// 適度にジャンプする
	//jumpTimer_ -= MAGISYSTEM::GetDeltaTime();
	//if (jumpTimer_ <= 0.0) {
	//	enemyAI->Jump();
	//	jumpTimer_ = jumpTime_;
	//}
}

void EnemyAIStateRoot::Exit(EnemyAI* enemyAI) {

}
