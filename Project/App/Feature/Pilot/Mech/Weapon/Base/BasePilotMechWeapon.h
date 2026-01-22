#pragma once

#include "Feature/Mech/Weapon/BaseMechWeapon.h"

class PilotMech;

/// <summary>
/// パイロット機体の武器ベースクラス
/// </summary>
class BasePilotMechWeapon :public BaseMechWeapon {
public:
	using BaseMechWeapon::BaseMechWeapon;
	~BasePilotMechWeapon() = default;

protected:
	PilotMech* GetPilotMech();

};