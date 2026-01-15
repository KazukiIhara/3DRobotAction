#pragma once

#include "GameObject/Boss/Mech/Parts/Arm/Base/BossMechBaseArm.h"

/// <summary>
/// ボス左腕クラス
/// </summary>
class BossMechLeftArm :public BossMechBaseArm {
public:
	BossMechLeftArm(const BossMechBaseArm::InitParam& param, BossMech* mech);
	~BossMechLeftArm() = default;

};