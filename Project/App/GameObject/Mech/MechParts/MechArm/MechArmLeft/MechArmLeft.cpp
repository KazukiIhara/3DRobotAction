#include "MechArmLeft.h"

#include "GameObject/Mech/MechCore/MechCore.h"

MechArmLeft::MechArmLeft(const Param& param)
	: BaseMechArm(param) {
}

Vector3 MechArmLeft::GetFireOffsetWorldPos(MechCore* mechCore) const {
	return mechCore->GetLeftHandWeapon()->GetData().fireOffsetWorldPos;
}

float MechArmLeft::GetBulletSpeed(MechCore* mechCore) const {
	return mechCore->GetLeftHandWeapon()->GetParam().speed;
}
