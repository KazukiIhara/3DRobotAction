#include "MechWeaponAssultRifle.h"

#include "3D/GameObject3D/GameObject3D.h"
#include "3D/Transform3D/Transform3D.h"

MechWeaponAssultRifle::MechWeaponAssultRifle()
	:BaseMechWeapon("AssultRifle", WeaponType::AssultRifle) {
	// 初期座標を設定
	if (auto obj = weapon_.lock()) {
		obj->GetTransform()->SetTranslateZ(0.6f);
	}
}

void MechWeaponAssultRifle::Update(MechCore* mechCore) {
	

}

Vector3 MechWeaponAssultRifle::GetFireWorldPosition() {
	return Vector3();
}
