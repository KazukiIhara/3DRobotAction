#include "BossMechStateDestroy.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "MAGI.h"

#include "Feature/Boss/Mech/BossMech.h"
#include "Feature/Boss/Mech/Weapon/LaserBlade/BossMechWeaponLaserBlade.h"
#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

using namespace Magi;

void BossMechStateDestroy::Enter([[maybe_unused]] BossMech* mech) {
	auto ms = mech->GetMoveSystem();
	ms->Reset();
}

void BossMechStateDestroy::Update([[maybe_unused]] BossMech* mech) {

}

void BossMechStateDestroy::Exit([[maybe_unused]] BossMech* mech) {

}
