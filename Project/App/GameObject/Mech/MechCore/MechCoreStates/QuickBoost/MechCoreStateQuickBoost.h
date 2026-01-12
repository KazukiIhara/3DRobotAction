#pragma once

// MyHedder
#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// 移動
/// </summary>
class MechCoreStateQuickBoost :public MechCoreBaseState {
public:
	MechCoreStateQuickBoost();
	~MechCoreStateQuickBoost()override = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;
private:
	bool JustDodgeUpdate();
private:
	// ジャスト回避判定タイマー
	float justDodgeTimer_{};
};