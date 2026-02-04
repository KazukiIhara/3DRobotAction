#define NOMINMAX

#include "PilotMechWeaponMachineGun.h"

// パイロット機体
#include "Feature/Pilot/Mech/PilotMech.h"

// マネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"

// 攻撃オブジェクト
#include "Feature/Damage/Object/ProjectileAttack/Bullet/Bullet.h"

// フレームワーク
#include "MAGI.h"

using namespace Magi;

PilotMechWeaponMachineGun::PilotMechWeaponMachineGun(PilotMech* mech) :
	BasePilotMechWeapon(mech) {

	// モデルロード
	MAGISYSTEM::LoadCreateModel("PilotMechMachineGun");

	// 発射位置オフセット設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Pilot","MachineGun","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

	// 装弾数セット
	const int32_t maxAmmo = MAGISYSTEM::GetParameterValue<int32_t>({ "WeaponParam","Pilot","MachineGun","MaxAmmo" });
	ammo_ = maxAmmo;

	// 手に紐づける
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::HandRight), false);

}

void PilotMechWeaponMachineGun::Update() {
	// 発射レート計算
	const float dt = MAGISYSTEM::GetDeltaTime();
	coolTimer_ -= dt;
	coolTimer_ = std::max(0.0f, coolTimer_);

	// 発射位置オフセット設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Pilot","MachineGun","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);
}

void PilotMechWeaponMachineGun::Draw() {
	MAGISYSTEM::DrawModel("PilotMechMachineGun", transform_->GetWorldMatrix(), mat_);
}

void PilotMechWeaponMachineGun::Attack([[maybe_unused]] Damage::Power power) {

	// クールタイム中なら早期リターン
	if (coolTimer_ > 0.0f) {
		return;
	}

	// 残弾が0なら早期リターン
	if (ammo_ <= 0) {
		return;
	}

	// 各種パラメータ取得
	const Vector3 shotPos = fireTransform_->GetWorldPosition();
	const Vector3 targetPos = mech_->GetTargetWorldPos();

	const Vector3 dir = Normalize(targetPos - shotPos);
	const float length = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","MachineGun","Length" });
	const float life = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","MachineGun","Life" });
	const float speed = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","MachineGun","Speed" });
	const float thickness = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Pilot","MachineGun","Thickness" });
	const int32_t damage = MAGISYSTEM::GetParameterValue<int32_t>({ "WeaponParam","Pilot","MachineGun","Damage" });

	// 攻撃作成、追加処理
	if (auto atkM = mech_->GetDamageObjectManager()) {
		// パラメータを作成
		Bullet::InitParam param{};
		param.initParam_.emitPos = shotPos;
		param.initParam_.dir = dir;
		param.initParam_.length = length;
		param.initParam_.life = life;
		param.initParam_.speed = speed;
		param.initParam_.thickness = thickness;

		param.initParam_.gParam.damage = damage;
		param.initParam_.gParam.power = Damage::Power::Small;
		param.initParam_.gParam.tag = FriendlyTag::PlayerSide;

		// 参照ポインタ構造体を作成
		Bullet::RefContext ref{
			.damageCollisionSystem = mech_->GetDamageCollisionSystem(),
			.effectManager = mech_->GetGameEffectManager(),
		};

		// 弾を追加
		std::unique_ptr<Bullet> beam = std::make_unique<Bullet>(param, ref);
		atkM->Add(std::move(beam));

	}

	// 弾数を減らす
	ammo_--;

	// 発射レートのタイマー
	const int32_t fireRateSec = MAGISYSTEM::GetParameterValue<int32_t>({ "WeaponParam","Pilot","MachineGun","FireRateSec" });
	const float coolTime = 1.0f / static_cast<float>(fireRateSec);
	coolTimer_ = coolTime;

}

void PilotMechWeaponMachineGun::AddAmmo(int32_t ammo) {
	ammo_ = ammo;
	const int32_t maxAmmo = MAGISYSTEM::GetParameterValue<int32_t>({ "WeaponParam","Pilot","MachineGun","MaxAmmo" });
	ammo_ = std::max(ammo_, maxAmmo);
}
