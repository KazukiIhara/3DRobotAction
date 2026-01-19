#pragma once

#include "Feature/Mech/Weapon/BaseMechWeapon.h"
#include "Structs/ModelStruct.h"

// 実装メモ
/*
	レーザー銃
	攻撃判定とエフェクトを発生させる
*/

class BossMech;

/// <summary>
/// ボス武器レーザー銃
/// </summary>
class BossMechWeaponLaserGun:public BaseMechWeapon {
public:
	BossMechWeaponLaserGun(BossMech* mech);
	~BossMechWeaponLaserGun() = default;

	void Update()override;
	void Draw()override;

	void Attack()override;
private:
	ModelMaterial mat_;
};