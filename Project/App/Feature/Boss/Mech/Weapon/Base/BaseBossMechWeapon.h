#pragma once

#include "Feature/Mech/Weapon/BaseMechWeapon.h"

class BossMech;

/// <summary>
/// ボスの武器ベースクラス
/// </summary>
class BaseBossMechWeapon :public BaseMechWeapon {
public:
	using BaseMechWeapon::BaseMechWeapon;
	virtual ~BaseBossMechWeapon() = default;

protected:
	BossMech* GetBossMech();
};