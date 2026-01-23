#include "PilotMechWeaponBeamCannonRifle.h"

#include "Feature/Pilot/Mech/PilotMech.h"

// マネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"


// 攻撃オブジェクト
#include "Feature/Damage/Object/ProjectileAttack/BeamCannon/BeamCannon.h"

// フレームワーク
#include "MAGI.h"

using namespace Magi;

PilotMechWeaponBeamCannonRifle::PilotMechWeaponBeamCannonRifle(PilotMech* mech) :
	BasePilotMechWeapon(mech) {

	// モデルをロード


	// オフセットの設定


	// 手に紐づける
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::HandLeft), false);

}

void PilotMechWeaponBeamCannonRifle::Update() {

}

void PilotMechWeaponBeamCannonRifle::Draw() {

}

void PilotMechWeaponBeamCannonRifle::Attack() {
	// 発射座標
	const Vector3 shotPos = fireTransform_->GetWorldPosition();
	// ターゲットの座標
	const Vector3 targetPos = {};
	// 方向を計算
	const Vector3 dir = Normalize(targetPos - shotPos);

}
