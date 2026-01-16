#include "BossMechWeaponLaserGun.h"

#include "GameObject/Boss/Mech/BossMech.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"

void BossMechWeaponLaserGun::Update() {
	
}

void BossMechWeaponLaserGun::Draw() {
	
}

void BossMechWeaponLaserGun::Attack() {
	// 攻撃オブジェクトマネージャを取得
	if (auto atkM = mech_->GetDamageObjectManager()) {
		
	}
}