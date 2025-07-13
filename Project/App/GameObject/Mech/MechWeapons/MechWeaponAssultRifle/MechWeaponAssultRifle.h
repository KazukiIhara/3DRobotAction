#pragma once

#include "GameObject/Mech/MechWeapons/BaseMechWeapon/BaseMechWeapon.h"

/// <summary>
/// ライフル武器クラス
/// </summary>
class MechWeaponAssultRifle: public BaseMechWeapon {
public:
	MechWeaponAssultRifle();
	~MechWeaponAssultRifle()override = default;

	void Update(MechCore* mechCore)override;

};