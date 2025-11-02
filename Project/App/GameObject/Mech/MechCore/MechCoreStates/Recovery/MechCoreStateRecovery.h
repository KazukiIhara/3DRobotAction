#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// 硬直ステート
/// </summary>
class MechCoreStateRecovery:public MechCoreBaseState {
public:
	~MechCoreStateRecovery() = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;

private:
	float recoveryTime_ = 0.0f;
};