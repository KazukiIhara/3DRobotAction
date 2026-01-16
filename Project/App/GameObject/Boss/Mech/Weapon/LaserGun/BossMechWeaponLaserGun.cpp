#include "BossMechWeaponLaserGun.h"

#include "GameObject/Boss/Mech/BossMech.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "GameObject/AttackObjectManager/AttackObjectManager.h"

void BossMechWeaponLaserGun::Update() {
	
}

void BossMechWeaponLaserGun::Draw() {
	
}

void BossMechWeaponLaserGun::Attack() {
	// 攻撃オブジェクトマネージャを取得
	if (auto atkM = mech_->GetAttackObjectManager()) {
		
	}
}