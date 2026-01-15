#pragma once

#include "GameObject/Boss/Mech/Parts/Leg/Base/BossMechBaseLeg.h"

/// <summary>
/// ボス機体右足
/// </summary>
class BossMechRightLeg :public BossMechBaseLeg {
public:
	BossMechRightLeg(const BossMechBaseLeg::InitParam& param, BossMech* mech);
	~BossMechRightLeg() = default;

};