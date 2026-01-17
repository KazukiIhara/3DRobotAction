#include "BossMechWeaponLaserGun.h"

// ボス機体クラス
#include "GameObject/Boss/Mech/BossMech.h"

// マネージャ
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"

// レーザー
#include "GameObject/Damage/Object/Laser/Laser.h"
// レーザーエフェクト
#include "GameEffects/LaserEffect/LaserEffect.h"

// プレイヤーの機体参照用
#include "GameObject/Mech/MechCore/MechCore.h"

// フレームワーク
#include "MAGI.h"

using namespace Magi;

BossMechWeaponLaserGun::BossMechWeaponLaserGun(BossMech* mech) :
	BossMechBaseWeapon(mech) {

}

void BossMechWeaponLaserGun::Update() {

}

void BossMechWeaponLaserGun::Draw() {

}

void BossMechWeaponLaserGun::Attack() {
	// 発射座標
	const Vector3 shotPos = fireTransform_->GetWorldPosition();
	// プレイヤーの座標を取得
	const Vector3 playerPos = mech_->GetPlayerMech()->GetBodyWPos();
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
		lParam.emitPos = shotPos;
		lParam.dir = dir;
		lParam.speed = speed;
		lParam.life = life;

		// レーザーを追加
		std::unique_ptr<Laser> laser = std::make_unique<Laser>(lParam);
		atkM->Add(std::move(laser));
	}

	// エフェクト発生処理
	if (auto efcM = mech_->GetGameEffectManager()) {
		LaserEffect::InitParam eParam{};
		eParam.emitPos = shotPos;
		eParam.dir = dir;
		eParam.speed = speed;
		eParam.life = life;

		// レーザーエフェクト追加
		std::unique_ptr<LaserEffect> laserEffect = std::make_unique<LaserEffect>(eParam);
		efcM->Add(std::move(laserEffect));
	}

}