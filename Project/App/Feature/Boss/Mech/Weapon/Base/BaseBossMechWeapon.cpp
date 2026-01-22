#include "BaseBossMechWeapon.h"

#include "Feature/Boss/Mech/BossMech.h"

BossMech* BaseBossMechWeapon::GetBossMech() {
	return dynamic_cast<BossMech*>(mech_);
}
