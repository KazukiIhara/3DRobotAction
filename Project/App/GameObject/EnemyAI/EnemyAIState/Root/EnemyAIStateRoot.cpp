#include "EnemyAIStateRoot.h"

#include "MAGI.h"

#include "GameObject/EnemyAI/EnemyAI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void EnemyAIStateRoot::Enter(EnemyAI* enemyAI, MechCore* mechCore) {
	jumpTimer_ = jumpTime_;
}

void EnemyAIStateRoot::Update(EnemyAI* enemyAI, MechCore* mechCore) {
	// 旋回行動
	enemyAI->MoveDir(Vector2(1.0f, 1.0f));

	// 右手武器を見る(近接でなければ撃つ)
	if (mechCore->GetRightHandWeapon()->GetType() == WeaponType::AssultRifle) {
		enemyAI->RightHandWeapon();
	}

	// 左手武器を見る(近接でなければ撃つ)
	if (mechCore->GetLeftHandWeapon()->GetType() == WeaponType::AssultRifle) {
		enemyAI->LeftHandWeapon();
	}

	// 適度にジャンプする
	jumpTimer_ -= MAGISYSTEM::GetDeltaTime();
	if (jumpTimer_ <= 0.0) {
		enemyAI->Jump();
		jumpTimer_ = jumpTime_;
	}
}

void EnemyAIStateRoot::Exit(EnemyAI* enemyAI, MechCore* mechCore) {

}
