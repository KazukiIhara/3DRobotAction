#pragma once

#include "Feature/Boss/Mech/Weapon/Base/BaseBossMechWeapon.h"
#include "Structs/ModelStruct.h"

class BossMechWeaponLaserBlade:public BaseBossMechWeapon {
public:
	BossMechWeaponLaserBlade(BossMech* mech);
	~BossMechWeaponLaserBlade()override = default;
	void Update()override;
	void Draw()override;

	void Attack(Damage::Power power)override;
private:
	ModelMaterial mat_;
};