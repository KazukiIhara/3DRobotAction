#include "BossMechStateDestroy.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "MAGI.h"

#include "Feature/Boss/Mech/BossMech.h"
#include "Feature/Boss/Mech/Weapon/LaserBlade/BossMechWeaponLaserBlade.h"
#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "Feature/Effect/BossDestroy/BossDestroyEffect.h"

using namespace Magi;

void BossMechStateDestroy::Enter([[maybe_unused]] BossMech* mech) {
	auto ms = mech->GetMoveSystem();
	ms->Reset();
	mech->GetAnimator()->PlayAnimation("Boss_Destroy", 1.0f, 1.0f);
	mech->GetLegLandingSystem()->SetEnable(false);

	// 撃破時エフェクト追加
	auto effectMgr = mech->GetGameEffectManager();
	effectMgr->Add(std::make_unique<BossDestroyEffect>(mech->GetCenterPos(), effectMgr));

}

void BossMechStateDestroy::Update([[maybe_unused]] BossMech* mech) {
	mech->GetAnimator()->SetLegAnimationEnabled(true);
	mech->GetAnimator()->SetAutoDisableLegOnGround(false);

	auto& lookFlag = mech->GetRotControlSystem()->GetLookAtFlag();
	lookFlag.mech.yaw = false;
}

void BossMechStateDestroy::Exit([[maybe_unused]] BossMech* mech) {

}
