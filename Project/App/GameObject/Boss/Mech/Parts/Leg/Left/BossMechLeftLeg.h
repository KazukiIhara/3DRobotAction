#pragma once

#include "GameObject/Boss/Mech/Parts/Leg/Base/BossMechBaseLeg.h"

/// <summary>
/// ボス機体左足
/// </summary>
class BossMechLeftLeg :public BossMechBaseLeg {
public:
	BossMechLeftLeg(const BossMechBaseLeg::InitParam& param, BossMech* mech);
	~BossMechLeftLeg() = default;

};