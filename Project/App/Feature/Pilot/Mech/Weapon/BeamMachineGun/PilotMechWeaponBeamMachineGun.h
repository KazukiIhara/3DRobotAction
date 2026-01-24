#pragma once

// C++
#include <cstdint>

#include "Feature/Pilot/Mech/Weapon/Base/BasePilotMechWeapon.h"
#include "Structs/ModelStruct.h"

/// <summary>
/// パイロット機体のビームマシンガン
/// </summary>
class PilotMechWeaponBeamMachineGun:public BasePilotMechWeapon {
public:
	PilotMechWeaponBeamMachineGun(PilotMech* mech);
	~PilotMechWeaponBeamMachineGun() = default;

	void Update()override;
	void Draw()override;

	void Attack()override;

private:
	float coolTimer_ = 0.0f;

	ModelMaterial mat_;
};