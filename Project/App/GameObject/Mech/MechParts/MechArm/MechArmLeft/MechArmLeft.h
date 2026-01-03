#pragma once

#include "GameObject/Mech/MechParts/MechArm/BaseMechArm/BaseMechArm.h"

/// <summary>
/// 機体の左手クラス
/// </summary>
class MechArmLeft final : public BaseMechArm {
public:
	MechArmLeft(const Param& param);
	~MechArmLeft() = default;

private:

	Vector3 GetFireOffsetWorldPos(MechCore* mechCore) const override;
	float GetBulletSpeed(MechCore* mechCore) const override;
};
