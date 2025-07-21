#pragma once

#include "GameObject/Mech/MechWeapons/BaseMechWeapon/BaseMechWeapon.h"
#include "Math/Utility/MathUtility.h"

/// <summary>
/// ライフル武器クラス
/// </summary>
class MechWeaponAssultRifle: public BaseMechWeapon {
public:
	MechWeaponAssultRifle();
	~MechWeaponAssultRifle()override = default;

	void Update(MechCore* mechCore)override;

	// 射撃時に弾が発射される座標を取得
	Vector3 GetFireWorldPosition()override;

	void SetReloadTime()override;
	void SetCoolTime()override;

private:
	Vector3 fireLocalPosition_ = { 0.0f,0.24f,1.3f };
	Matrix4x4 fireLocalTranslateMatrix_;
	Vector3 fireWorldPosition_ = { 0.0f,0.0f,0.0f };

	// 発射感覚
	const float coolTime_ = 0.5f;\
};