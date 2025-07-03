#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// 待機状態
/// </summary>
class MechCoreStateIdle :public MechCoreBaseState {
public:
	~MechCoreStateIdle()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;

private:

};