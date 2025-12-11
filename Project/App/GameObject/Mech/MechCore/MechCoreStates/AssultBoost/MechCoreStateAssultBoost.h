#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"


namespace magi {
/// <summary>
/// 機体アサルトブーストステート
/// </summary>
class MechCoreStateAssultBoost :public MechCoreBaseState {
public:
	~MechCoreStateAssultBoost()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;

};
}
