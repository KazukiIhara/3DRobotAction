#pragma once

#include "GameObject/Mech/MechParts/MechArm/BaseMechArm/BaseMechArm.h"

/// <summary>
/// 機体の右手クラス
/// </summary>
class MechArmRight final : public BaseMechArm {
public:
	MechArmRight(const Param& param);
	~MechArmRight() = default;

private:

	Vector3 GetFireOffsetWorldPos(MechCore* mechCore) const override;
	float GetBulletSpeed(MechCore* mechCore) const override;
};
