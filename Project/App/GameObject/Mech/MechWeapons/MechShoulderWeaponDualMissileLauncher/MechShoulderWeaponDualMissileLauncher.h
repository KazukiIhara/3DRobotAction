#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"

// BaseClass
#include "GameObject/Mech/MechWeapons/BaseMechShoulderWeapon/BaseMechShoulderWeapon.h"

class MechShoulderWeaponDualMissileLauncher:public BaseMechShoulderWeapon {
public:
	MechShoulderWeaponDualMissileLauncher(const WitchShoulder& witch);
	~MechShoulderWeaponDualMissileLauncher()override = default;

	void Update(MechCore* mechCore)override;


private:
	// 初速
	float firstSpeed_ = 0.0f;
	// 加速度
	float acc_ = 0.0f;

	// 胴体からのオフセット
	Vector3 offset_ = { 0.3f,0.3f,-0.3f };
};
