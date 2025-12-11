#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"


namespace magi {
/// <summary>
/// 移動
/// </summary>
class MechCoreStateQuickBoost :public MechCoreBaseState {
public:
	~MechCoreStateQuickBoost()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;
};
}
