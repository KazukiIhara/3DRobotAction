#pragma once

#include "GameObject/Boss/Mech/Parts/Arm/Base/BossMechBaseArm.h"

/// <summary>
/// ボス右腕クラス
/// </summary>
class BossMechRightArm :public BossMechBaseArm {
public:
	BossMechRightArm(const BossMechBaseArm::InitParam& param, BossMech* mech);
	~BossMechRightArm() = default;

};
