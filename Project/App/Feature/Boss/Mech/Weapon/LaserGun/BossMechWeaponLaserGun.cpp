#include "BossMechWeaponLaserGun.h"

// ボス機体クラス
#include "Feature/Boss/Mech/BossMech.h"

// マネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"

// レーザー
#include "Feature/Damage/Object/Laser/Laser.h"
// レーザーエフェクト
#include "Feature/Effect/LaserEffect/LaserEffect.h"

// プレイヤーの機体参照用
#include "Feature/Pilot/Mech/PilotMech.h"

// フレームワーク
#include "MAGI.h"

using namespace Magi;

BossMechWeaponLaserGun::BossMechWeaponLaserGun(BossMech* mech) :
	BaseBossMechWeapon(mech) {

	MAGISYSTEM::LoadCreateModel("BossMechLaserGun");

	// 発射位置オフセットを設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "BossMechWeapon","LaserGun","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

	// 手に紐づける
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::HandRight), false);

}

void BossMechWeaponLaserGun::Update() {
	// オフセットを設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "BossMechWeapon","LaserGun","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

}

void BossMechWeaponLaserGun::Draw() {
	MAGISYSTEM::DrawModel("BossMechLaserGun", transform_->GetWorldMatrix(), mat_);
}

void BossMechWeaponLaserGun::Attack() {
	// 発射座標
	const Vector3 shotPos = fireTransform_->GetWorldPosition();
	// プレイヤーの座標を取得
	const Vector3 playerPos = GetBossMech()->GetPilotMech()->GetCenterPos();
	// 方向を計算
	const Vector3 dir = Normalize(playerPos - shotPos);
	// 弾速取得
	const float speed = MAGISYSTEM::GetParameterValue<float>({ "BossMechWeapon","LaserGun","Speed" });
	// 生存時間取得
	const float life = MAGISYSTEM::GetParameterValue<float>({ "BossMechWeapon","LaserGun","Life" });

	// 攻撃作成、追加処理
	if (auto atkM = mech_->GetDamageObjectManager()) {
		// レーザーのパラメータ作成
		Laser::InitParam lParam{};
		lParam.initParam_.emitPos = shotPos;
		lParam.initParam_.dir = dir;
		lParam.initParam_.speed = speed;
		lParam.initParam_.life = life;
		lParam.initParam_.tag = FriendlyTag::EnemySide;
		lParam.color = LaserEffect::Col::RED;

		// レーザーを追加
		Laser::RefContext ref{
			.damageCollisionSystem = mech_->GetDamageCollisionSystem(),
			.effectManager = mech_->GetGameEffectManager()
		};
		std::unique_ptr<Laser> laser = std::make_unique<Laser>(lParam, ref);
		atkM->Add(std::move(laser));
	}

}