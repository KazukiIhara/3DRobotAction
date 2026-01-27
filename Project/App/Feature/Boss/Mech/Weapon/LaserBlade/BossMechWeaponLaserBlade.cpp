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

	// ブレード発生地点のトランスフォーム
	bladeEmit_ = MAGISYSTEM::AddTransform3D();

	bladeEmit_->SetParent(transform_, false);
	const Vector3 bladeEmitOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Boss","LaserBlade","BladeOffset" });
	bladeEmit_->SetTranslate(bladeEmitOffset);
	bladeEmit_->SetRotateX(std::numbers::pi_v<float>);


	// ブレード設定
	inner_.height = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","InnerLength" });
	inner_.bottomRadius = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","InnerRadius" });
	inner_.topRadius = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","InnerRadius" });

	outer_.height = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","OuterLength" });
	outer_.bottomRadius = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","OuterRadius" });
	outer_.topRadius = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","OuterRadius" });

	innerMat_.textureName = "white.png";
	innerMat_.baseColor = MAGISYSTEM::GetParameterValue<Vector4>({ "WeaponParam","Boss","LaserBlade","InnerColor" });
	innerMat_.blendMode = BlendMode::Add;

	outerMat_.textureName = "white.png";
	outerMat_.baseColor = MAGISYSTEM::GetParameterValue<Vector4>({ "WeaponParam","Boss","LaserBlade","OuterColor" });
	outerMat_.blendMode = BlendMode::Add;

	inner_.height = 0.0f;
	outer_.height = 0.0f;
	inner_.bottomRadius = 0.0f;
	inner_.topRadius = 0.0f;
	outer_.bottomRadius = 0.0f;
	outer_.topRadius = 0.0;
}

void BossMechWeaponLaserBlade::Update() {
	// 発射位置オフセットを設定
	const Vector3 fireOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Boss","LaserBlade","FireOffset" });
	fireTransform_->SetTranslate(fireOffset);

	// ブレードの発生位置オフセット設定
	const Vector3 bladeEmitOffset = MAGISYSTEM::GetParameterValue<Vector3>({ "WeaponParam","Boss","LaserBlade","BladeOffset" });
	bladeEmit_->SetTranslate(bladeEmitOffset);


}

void BossMechWeaponLaserBlade::Draw() {
	MAGISYSTEM::DrawCylinder3D(bladeEmit_->GetWorldMatrix(), inner_, innerMat_);
	MAGISYSTEM::DrawCylinder3D(bladeEmit_->GetWorldMatrix(), outer_, outerMat_);
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

void BossMechWeaponLaserBlade::SetBladeLength(float length) {
	inner_.height = length;
	outer_.height = length;
}

void BossMechWeaponLaserBlade::SetInnerRadius(float radius) {
	inner_.bottomRadius = radius;
	inner_.topRadius = radius;
}

void BossMechWeaponLaserBlade::SetOuterRadius(float radius) {
	outer_.bottomRadius = radius;
	outer_.topRadius = radius;
}

float BossMechWeaponLaserBlade::GetOuterRad() const {
	return outer_.topRadius;
}

float BossMechWeaponLaserBlade::GetInnerRad() const {
	return inner_.topRadius ;
}

float BossMechWeaponLaserBlade::GetLength() const {
	return inner_.height ;
}
