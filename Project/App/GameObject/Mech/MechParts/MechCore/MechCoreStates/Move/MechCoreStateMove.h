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
	const float kMaxSpeed_ = 15.0f;

	// 秒間加速量
	const float moveAcc_ = 30.0f;

	float moveSpeed_ = 0.0f;
};