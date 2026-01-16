#pragma once

#include "GameObject/Boss/Mech/Weapon/Base/BossMechBaseWeapon.h"

// 実装メモ
/*
	レーザー銃
	攻撃判定とエフェクトを発生させる
*/

/// <summary>
/// ボス武器レーザー銃
/// </summary>
class BossMechWeaponLaserGun:public BossMechBaseWeapon {
public:
	using BossMechBaseWeapon::BossMechBaseWeapon;
	~BossMechWeaponLaserGun() = default;

	void Update()override;
	void Draw()override;

	void Attack()override;


};