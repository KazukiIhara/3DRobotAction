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
}

void MechShoulderWeaponDualMissileLauncher::Update([[maybe_unused]] MechCore* mechCore) {


	// クールタイム中かどうか
	if (coolTimer_ > 0.0f) {
		coolTimer_ -= MAGISYSTEM::GetDeltaTime();
		isCoolTime_ = true;
	} else {
		coolTimer_ = 0.0f;
		isCoolTime_ = false;
	}
}
