#include "BossMechStateIdle.h"

#include "GameObject/Boss/Mech/BossMech.h"

#include "MAGI.h"

using namespace Magi;

void BossMechStateIdle::Enter([[maybe_unused]] BossMech* mech) {
	idleAnimationT_ = 0.0f;
}

void BossMechStateIdle::Update([[maybe_unused]] BossMech* mech) {
	/*idleAnimationT_ +=MAGISYSTEM::GetDeltaTime();
	mech->GetAnimator()->ApplyAnimation("BossIdle", idleAnimationT_);*/
}

void BossMechStateIdle::Exit([[maybe_unused]] BossMech* mech) {

}
