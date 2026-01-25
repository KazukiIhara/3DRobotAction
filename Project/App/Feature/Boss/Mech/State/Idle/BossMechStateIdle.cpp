#include "BossMechStateIdle.h"

#include "Feature/Boss/Mech/BossMech.h"
#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

void BossMechStateIdle::Enter([[maybe_unused]] BossMech* mech) {
	mech->GetAnimator()->PlayAnimation("Boss_Idle", 1.0f, 1.0f);
}

void BossMechStateIdle::Update([[maybe_unused]] BossMech* mech) {
	// 機体をターゲットに向ける
	auto& lookFlag = mech->GetRotControlSystem()->GetLookAtFlag();
	lookFlag.mech.yaw = true;

}

void BossMechStateIdle::Exit([[maybe_unused]] BossMech* mech) {

}
