#pragma once

// C++
#include <cstdint>

#include "Feature/Pilot/Mech/Weapon/Base/BasePilotMechWeapon.h"
#include "Structs/ModelStruct.h"

/// <summary>
/// パイロット機体のマシンガン
/// </summary>
class PilotMechWeaponMachineGun:public BasePilotMechWeapon {
public:
	PilotMechWeaponMachineGun(PilotMech* mech);
	~PilotMechWeaponMachineGun() = default;

	void Update()override;
	void Draw()override;

	void Attack(Damage::Power power)override;

	void AddAmmo(int32_t ammo);

	int32_t GetAmmo()const;

private:
	int32_t ammo_ = 0;
	float coolTimer_ = 0.0f;

	ModelMaterial mat_;
};