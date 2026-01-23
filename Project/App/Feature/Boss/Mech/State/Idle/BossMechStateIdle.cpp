#include "BossMechStateIdle.h"

#include "Feature/Boss/Mech/BossMech.h"
#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

void BossMechStateIdle::Enter([[maybe_unused]] BossMech* mech) {

}

void BossMechStateIdle::Update([[maybe_unused]] BossMech* mech) {
	// 機体をターゲットに向ける
	auto& lookFlag = mech->GetRotControlSystem()->GetLookAtFlag();
	lookFlag.mech.yaw = true;

}

void BossMechStateIdle::Exit([[maybe_unused]] BossMech* mech) {

}
