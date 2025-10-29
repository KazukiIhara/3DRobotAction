#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// ロケラン等の反動ステート
/// </summary>
class MechCoreStateAftershock:public MechCoreBaseState {
public:
	~MechCoreStateAftershock()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;
private:

};