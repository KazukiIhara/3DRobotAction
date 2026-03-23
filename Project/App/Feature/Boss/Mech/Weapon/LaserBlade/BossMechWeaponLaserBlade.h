#pragma once

#include "Feature/Boss/Mech/Weapon/Base/BaseBossMechWeapon.h"
#include "Structs/ModelStruct.h"
#include "Structs/Primitive3DStruct.h"

class BossMechWeaponLaserBlade:public BaseBossMechWeapon {
public:
	BossMechWeaponLaserBlade(BossMech* mech);
	~BossMechWeaponLaserBlade()override = default;
	void Update()override;
	void Draw()override;

	void Attack(Damage::Power power)override;
	void SetBladeLength(float length);
	void SetInnerRadius(float radius);
	void SetOuterRadius(float radius);

	float GetOuterRad()const;
	float GetInnerRad()const;

	float GetLength()const;
private:
	ModelMaterial mat_;
	// ブレードの根本
	Magi::Transform3D* bladeEmit_;

	CylinderData3D inner_;
	CylinderData3D outer_;

	MaterialData3D innerMat_;
	MaterialData3D outerMat_;
};