#pragma once

// MyHedder
#include "GameObject/Mech/MechParts/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// 移動
/// </summary>
class MechCoreStateMove :public MechCoreBaseState {
public:
	~MechCoreStateMove()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;

private:
	const float kMaxSpeed_ = 3.0f;
	const float moveAcc_ = 3.0f;

	float moveSpeed_ = 0.0f;
};