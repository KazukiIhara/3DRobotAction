#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"

// BaseClass
#include "GameObject/Mech/MechWeapons/BaseMechShoulderWeapon/BaseMechShoulderWeapon.h"

class MechShoulderWeaponDualMissileLauncher :public BaseMechShoulderWeapon {
public:
	MechShoulderWeaponDualMissileLauncher(const WitchShoulder& witch);
	~MechShoulderWeaponDualMissileLauncher()override = default;

	void Update(MechCore* mechCore)override;

private:
	// ミサイルの初速
	const float kFirstSpeed_ = 10.0f;
	// ミサイルの加速度
	const float kAcc_ = 5.0f;
	// ミサイルの最大速度
	const float kMaxSpeed_ = 15.0f;
	// ミサイルのダメージ
	const int32_t kDamage_ = 500;

	// 胴体からのオフセット
	Vector3 offset_ = { 0.3f,0.3f,-0.3f };
};
