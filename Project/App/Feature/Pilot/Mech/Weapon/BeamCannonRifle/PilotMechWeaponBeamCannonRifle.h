#pragma once

#include "Feature/Pilot/Mech/Weapon/Base/BasePilotMechWeapon.h"
#include "Structs/ModelStruct.h"

/// <summary>
/// パイロット機体のビームキャノンライフル
/// </summary>
class PilotMechWeaponBeamCannonRifle :public BasePilotMechWeapon {
public:
	PilotMechWeaponBeamCannonRifle(PilotMech* mech);
	~PilotMechWeaponBeamCannonRifle() = default;

	void Update()override;
	void Draw()override;

	void Attack(Damage::Power power)override;
	
private:
	ModelMaterial mat_;
};