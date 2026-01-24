#include "PilotMechWeaponBeamCannonRifle.h"

// パイロット機体
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
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Pilot","BeamCannon","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

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
	const Vector3 targetPos = mech_->GetTargetWorldPos();
	const Vector3 dir = Normalize(targetPos - shotPos);
	const float length = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Length" });
	const float life = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Life" });
	const float speed = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Speed" });
	const float thickness = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Thickness" });

	// 攻撃作成、追加処理
	if (auto atkM = mech_->GetDamageObjectManager()) {
		// ビームキャノンのパラメータを作成
		BeamCannon::InitParam param{};
		param.initParam_.emitPos = shotPos;
		param.initParam_.dir = dir;
		param.initParam_.length = length;
		param.initParam_.life = life;
		param.initParam_.speed = speed;
		param.initParam_.thickness = thickness;

		param.initParam_.tag = FriendlyTag::PlayerSide;

		// 参照ポインタ構造体を作成
		BeamCannon::RefContext ref{
			.damageCollisionSystem = mech_->GetDamageCollisionSystem(),
			.effectManager = mech_->GetGameEffectManager(),
		};

		// ビームキャノンを追加
		std::unique_ptr<BeamCannon> beamCannon = std::make_unique<BeamCannon>(param, ref);
		atkM->Add(std::move(beamCannon));

	}

}
