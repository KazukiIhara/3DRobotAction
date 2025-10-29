#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// ロケラン攻撃ステート
/// </summary>
class MechCoreStateRocketLauncherAttack:public MechCoreBaseState {
public:
	~MechCoreStateRocketLauncherAttack()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;
private:

};