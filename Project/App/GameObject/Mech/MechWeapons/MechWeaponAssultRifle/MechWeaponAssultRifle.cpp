#include "MechWeaponAssultRifle.h"

#include "MAGI.h"

MechWeaponAssultRifle::MechWeaponAssultRifle()
	:BaseMechHandWeapon("AssultRifle", HandWeaponType::AssultRifle) {
	// 初期座標を設定
	if (auto obj = weapon_.lock()) {
		obj->GetTransform()->SetTranslateZ(0.6f);
	}
	// ローカル移動量行列を計算
	fireLocalTranslateMatrix_ = MakeTranslateMatrix(fireLocalPosition_);

	// 弾速を設定
	bulletSpeed_ = 70.0f;

	// ダメージを設定
	damage_ = kDamage_;
}

void MechWeaponAssultRifle::Update([[maybe_unused]] MechCore* mechCore) {
	if (auto weapon = weapon_.lock()) {
		// 弾の発射位置を計算
		const Matrix4x4 wMat = fireLocalTranslateMatrix_ * weapon->GetTransform()->GetWorldMatrix();
		fireWorldPosition_ = ExtractionWorldPos(wMat);
	}

	// クールタイム中かどうか
	if (coolTimer_ > 0.0f) {
		coolTimer_ -= MAGISYSTEM::GetDeltaTime();
		isCoolTime_ = true;
	} else {
		coolTimer_ = 0.0f;
		isCoolTime_ = false;
	}

}

Vector3 MechWeaponAssultRifle::GetFireWorldPosition() {
	return fireWorldPosition_;
}

void MechWeaponAssultRifle::SetReloadTime() {

}

void MechWeaponAssultRifle::SetCoolTime() {
	coolTimer_ = kCoolTime_;
}
