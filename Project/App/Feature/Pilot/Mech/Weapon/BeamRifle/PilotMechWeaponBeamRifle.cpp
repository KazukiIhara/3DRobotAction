#include "PilotMechWeaponBeamRifle.h"

#include "Feature/Pilot/Mech/PilotMech.h"

// マネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"


// 攻撃オブジェクトやエフェクト


// フレームワーク
#include "MAGI.h"

using namespace Magi;

PilotMechWeaponBeamRifle::PilotMechWeaponBeamRifle(PilotMech* mech) :
	BasePilotMechWeapon(mech) {

	// モデルをロード


	// オフセットの設定


	// 手に紐づける
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::HandLeft), false);

}

void PilotMechWeaponBeamRifle::Update() {

}

void PilotMechWeaponBeamRifle::Draw() {

}

void PilotMechWeaponBeamRifle::Attack() {

}
