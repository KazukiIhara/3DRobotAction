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
	// 旋回行動
	enemyAI->MoveDir(currentMoveDir_);

	// 適度にジャンプする
	jumpTimer_ -= MAGISYSTEM::GetDeltaTime();
	if (jumpTimer_ <= 0.0) {
		enemyAI->Jump();
		jumpTimer_ = jumpTime_;
	}
}

void EnemyAIStateRoot::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
