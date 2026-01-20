#include "BaseBossMechWeapon.h"

#include "Feature/Boss/Boss.h"

BossMech* BaseBossMechWeapon::GetBossMech() {
	return dynamic_cast<BossMech*>(mech_);
}
