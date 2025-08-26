#include "EnemyAIStateSearch.h"

#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void EnemyAIStateSearch::Enter([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}

void EnemyAIStateSearch::Update([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {
	// ジャンプしたり左右に少し揺れながら自機の方に近づいてくる

	// 一定の距離になったら通常行動に移行

}

void EnemyAIStateSearch::Exit([[maybe_unused]] EnemyAI* enemyAI, [[maybe_unused]] MechCore* mechCore) {

}
