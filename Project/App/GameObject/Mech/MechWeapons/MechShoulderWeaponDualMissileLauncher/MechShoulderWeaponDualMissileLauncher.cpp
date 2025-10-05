#include "MechShoulderWeaponDualMissileLauncher.h"

#include "MAGI.h"

MechShoulderWeaponDualMissileLauncher::MechShoulderWeaponDualMissileLauncher(const WitchShoulder& witch)
	:BaseMechShoulderWeapon("DualMissileLauncher", ShoulderWeaponType::DualMissileLauncher, witch) {

	// 初期座標を設定
	if (auto obj = weapon_.lock()) {
		switch (witchShoulder_) {
		case WitchShoulder::Left:
			obj->GetTransform()->SetTranslateX(-offset_.x);
			break;
		case WitchShoulder::Right:
			obj->GetTransform()->SetTranslateX(offset_.x);
			break;
		}
		obj->GetTransform()->SetTranslateY(offset_.y);
		obj->GetTransform()->SetTranslateZ(offset_.z);
	}

	// ミサイルの初速を設定
	firstSpeed_ = kFirstSpeed_;

	// ミサイルの加速度を設定
	acc_ = kAcc_;

	// ミサイルの最大速度を設定
	maxSpeed_ = kMaxSpeed_;

	// ダメージを設定
	damage_ = kDamage_;


	// ミサイルの拡散角度を設定
	dualMissileAngles_[0] = -25.0f;
	dualMissileAngles_[1] = 25.0f;
	dualMissileAngles_[2] = -45.0f;
	dualMissileAngles_[3] = 45.0f;
}

void MechShoulderWeaponDualMissileLauncher::Update([[maybe_unused]] MechCore* mechCore) {
	// 攻撃発生地点を計算
	if (auto obj = weapon_.lock()) {
		fireWorldPosition_ = obj->GetTransform()->GetWorldPosition();
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
