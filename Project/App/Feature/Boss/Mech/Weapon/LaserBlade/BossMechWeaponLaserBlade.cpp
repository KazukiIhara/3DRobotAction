#include "BossMechWeaponLaserBlade.h"

// ボス機体クラス
#include "Feature/Boss/Mech/BossMech.h"

// マネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"

// 攻撃オブジェクト
#include "Feature/Damage/Object/Melee/LaserBlade/LaserBladeSlash.h"

// プレイヤーの機体参照用
#include "Feature/Pilot/Mech/PilotMech.h"

// フレームワーク
#include "MAGI.h"

using namespace Magi;

BossMechWeaponLaserBlade::BossMechWeaponLaserBlade(BossMech* mech) :
	BaseBossMechWeapon(mech) {

	// 発射位置オフセットを設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Boss","LaserBlade","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

	// 手に紐づける
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::HandRight), false);

}

void BossMechWeaponLaserBlade::Update() {
	// 発射位置オフセットを設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Boss","LaserBlade","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

}

void BossMechWeaponLaserBlade::Draw() {

}

void BossMechWeaponLaserBlade::Attack(Damage::Power power) {

	const float radius = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","Radius" });
	const float life = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","Life" });
	const int32_t damage = MAGISYSTEM::GetParameterValue<int32_t>({ "WeaponParam","Boss","LaserBlade","Damage" });

	// 攻撃作成、追加処理
	if (auto atkM = mech_->GetDamageObjectManager()) {
		LaserBladeSlash::InitParam mParam{};
		mParam.initParam_.parent = fireTransform_;
		mParam.initParam_.radius = radius;
		mParam.initParam_.life = life;

		mParam.initParam_.gParam.damage = damage;
		mParam.initParam_.gParam.power = power;
		mParam.initParam_.gParam.tag = FriendlyTag::EnemySide;

		// 参照ポインタ構造体を作成
		LaserBladeSlash::RefContext ref{
			.damageCollisionSystem = mech_->GetDamageCollisionSystem(),
			.effectManager = mech_->GetGameEffectManager()
		};

		// 攻撃を追加
		atkM->Add(std::move(std::make_unique<LaserBladeSlash>(mParam, ref)));

	}
}
