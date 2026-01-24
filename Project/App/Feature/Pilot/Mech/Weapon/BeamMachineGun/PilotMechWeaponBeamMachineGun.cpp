#define NOMINMAX

#include "PilotMechWeaponBeamMachineGun.h"

// パイロット機体
#include "Feature/Pilot/Mech/PilotMech.h"

// マネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"

// 攻撃オブジェクト
#include "Feature/Damage/Object/ProjectileAttack/Beam/Beam.h"

// フレームワーク
#include "MAGI.h"

using namespace Magi;

PilotMechWeaponBeamMachineGun::PilotMechWeaponBeamMachineGun(PilotMech* mech) :
	BasePilotMechWeapon(mech) {

	// モデルロード

	// オフセット設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Pilot","BeamMachineGun","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);


	// 手に紐づける
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::HandRight), false);

}

void PilotMechWeaponBeamMachineGun::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();
	coolTimer_ -= dt;
	coolTimer_ = std::max(0.0f, coolTimer_);
}

void PilotMechWeaponBeamMachineGun::Draw() {

}

void PilotMechWeaponBeamMachineGun::Attack() {

	if (coolTimer_ > 0.0f) {
		return;
	}

	const Vector3 shotPos = fireTransform_->GetWorldPosition();
	const Vector3 targetPos = mech_->GetTargetWorldPos();

	const Vector3 dir = Normalize(targetPos - shotPos);
	const float length = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamMachineGun","Length" });
	const float life = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamMachineGun","Life" });
	const float speed = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamMachineGun","Speed" });
	const float thickness = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamMachineGun","Thickness" });
	const float damage = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","BeamCannon","Damage" });

	// 攻撃作成、追加処理
	if (auto atkM = mech_->GetDamageObjectManager()) {
		// ビームのパラメータを作成
		Beam::InitParam param{};
		param.initParam_.emitPos = shotPos;
		param.initParam_.dir = dir;
		param.initParam_.length = length;
		param.initParam_.life = life;
		param.initParam_.speed = speed;
		param.initParam_.thickness = thickness;

		param.initParam_.gParam.damage = damage;
		param.initParam_.gParam.power = DamageCollider::Power::Small;
		param.initParam_.gParam.tag = FriendlyTag::PlayerSide;

		// 参照ポインタ構造体を作成
		Beam::RefContext ref{
			.damageCollisionSystem = mech_->GetDamageCollisionSystem(),
			.effectManager = mech_->GetGameEffectManager(),
		};

		// ビームキャノンを追加
		std::unique_ptr<Beam> beam = std::make_unique<Beam>(param, ref);
		atkM->Add(std::move(beam));

	}

	// タイマーセット
	const int32_t fireRateSec = MAGISYSTEM::GetParameterValue<int32_t>({ "WeaponParam","Pilot","BeamMachineGun","FireRateSec" });
	const float coolTime = 1.0f / static_cast<float>(fireRateSec);
	coolTimer_ = coolTime;

}
