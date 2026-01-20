#pragma once

#include "Feature/Mech/Weapon/BaseMechWeapon.h"

class BossMech;

class BaseBossMechWeapon :public BaseMechWeapon {
public:
	using BaseMechWeapon::BaseMechWeapon;
	virtual ~BaseBossMechWeapon() = default;

protected:
	BossMech* GetBossMech();
};