#pragma once

#include "Feature/Pilot/Mech/Weapon/Base/BasePilotMechWeapon.h"
#include "Structs/ModelStruct.h"

/// <summary>
/// パイロット機体のビームライフル
/// </summary>
class PilotMechWeaponBeamRifle :public BasePilotMechWeapon {
public:
	PilotMechWeaponBeamRifle(PilotMech* mech);
	~PilotMechWeaponBeamRifle() = default;

	void Update()override;
	void Draw()override;

	void Attack()override;
	
private:
	ModelMaterial mat_;
};