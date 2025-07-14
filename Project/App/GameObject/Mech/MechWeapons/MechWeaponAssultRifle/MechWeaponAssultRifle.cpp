#include "MechWeaponAssultRifle.h"

#include "MAGI.h"

MechWeaponAssultRifle::MechWeaponAssultRifle()
	:BaseMechWeapon("AssultRifle", WeaponType::AssultRifle) {
	// 初期座標を設定
	if (auto obj = weapon_.lock()) {
		obj->GetTransform()->SetTranslateZ(0.6f);
	}
	// ローカル移動量行列を計算
	fireLocalTranslateMatrix_ = MakeTranslateMatrix(fireLocalPosition_);
}

void MechWeaponAssultRifle::Update(MechCore* mechCore) {
	const Matrix4x4 wMat = fireLocalTranslateMatrix_ * weapon_.lock()->GetTransform()->GetWorldMatrix();
	fireWorldPosition_ = ExtractionWorldPos(wMat);
}

Vector3 MechWeaponAssultRifle::GetFireWorldPosition() {
	return fireWorldPosition_;
}