#pragma once

// MyHedder
#include "GameObject/Mech/MechParts/MechCore/MechCoreStates/MechCoreBaseState.h"

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