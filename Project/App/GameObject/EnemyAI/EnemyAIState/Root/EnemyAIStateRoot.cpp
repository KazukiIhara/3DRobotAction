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


	// 状態遷移用のスコアを取得


	// 状態遷移


}

void EnemyAIStateRoot::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
