#pragma once

#include "GameObject/Boss/Mech/Weapon/Base/BossMechBaseWeapon.h"
#include "Structs/ModelStruct.h"

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
	BossMechWeaponLaserGun(BossMech* mech);
	~BossMechWeaponLaserGun() = default;

	void Update()override;
	void Draw()override;

	void Attack()override;
private:
	ModelMaterial mat_;
};