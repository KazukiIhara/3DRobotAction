#include "BossMechStateIdle.h"

#include "GameObject/Boss/Mech/BossMech.h"

#include "MAGI.h"

using namespace Magi;

void BossMechStateIdle::Enter([[maybe_unused]] BossMech* mech) {
	idleAnimationT_ = 0.0f;
}

void BossMechStateIdle::Update([[maybe_unused]] BossMech* mech) {
	mech->GetAnimator()->ApplyAnimation("BossIdle", idleAnimationT_, 0.1f);
	idleAnimationT_ += MAGISYSTEM::GetDeltaTime() * (1.0f / 1.5f);

	idleAnimationT_ = std::min(1.0f, idleAnimationT_);
	if (idleAnimationT_ == 1.0f) {
		idleAnimationT_ = 0.0f;
	}
}

void BossMechStateIdle::Exit([[maybe_unused]] BossMech* mech) {

}
