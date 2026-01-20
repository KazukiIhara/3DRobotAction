#pragma once

#include "Feature/Boss/Mech/Weapon/Base/BaseBossMechWeapon.h"
#include "Structs/ModelStruct.h"

// 実装メモ
/*
	レーザー銃
	攻撃判定とエフェクトを発生させる
*/

/// <summary>
/// ボス武器レーザー銃
/// </summary>
class BossMechWeaponLaserGun :public BaseBossMechWeapon {
public:
	BossMechWeaponLaserGun(BossMech* mech);
	~BossMechWeaponLaserGun()override = default;

	void Update()override;
	void Draw()override;

	void Attack()override;
private:
	ModelMaterial mat_;
};