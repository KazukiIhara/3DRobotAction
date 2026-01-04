#include "MechArmRight.h"

#include "GameObject/Mech/MechCore/MechCore.h"

MechArmRight::MechArmRight(const Param& param)
	: BaseMechArm(param) {
}

Vector3 MechArmRight::GetFireOffsetWorldPos(MechCore* mechCore) const {
	return mechCore->GetRightHandWeapon()->GetData().fireOffsetWorldPos;
}

float MechArmRight::GetBulletSpeed(MechCore* mechCore) const {
	return mechCore->GetRightHandWeapon()->GetParam().speed;
}
