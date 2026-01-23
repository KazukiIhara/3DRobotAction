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

	// パラメータ
	const Vector3 shotPos = fireTransform_->GetWorldPosition();
	const Vector3 targetPos = {};
	const Vector3 dir = Normalize(targetPos - shotPos);
	const float length = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Length" });
	const float life = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Life" });
	const float speed = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Speed" });
	const float thickness = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Thickness" });
	
	// 攻撃作成、追加処理
	if (auto atkM = mech_->GetDamageObjectManager()) {
		// ビームキャノンのパラメータを作成
		BeamCannon::InitParam param{};
		param.initParam_.emitPos;
		param.initParam_.dir;
		param.initParam_.length;
		param.initParam_.life;
		param.initParam_.speed;
		param.initParam_.thickness;

		param.initParam_.tag = FriendlyTag::PlayerSide;

		// 参照ポインタ構造体を作成
		BeamCannon::RefContext ref{
			.damageCollisionSystem = mech_->GetDamageCollisionSystem(),
			.effectManager = mech_->GetGameEffectManager(),
		};

		// ビームキャノンを追加
		std::unique_ptr<BeamCannon> laser = std::make_unique<BeamCannon>(param, ref);
		atkM->Add(std::move(laser));

	}

}
