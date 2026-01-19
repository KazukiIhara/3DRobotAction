#pragma once

#include "Feature/Boss/Mech/State/BossMechBaseState.h"

// 実装メモ
/*
	待機アニメーションを再生する
*/

/// <summary>
/// 待機状態　
/// </summary>
class BossMechStateIdle :public BossMechBaseState {
public:
	~BossMechStateIdle()override = default;
	void Enter(BossMech* mech)override;
	void Update(BossMech* mech)override;
	void Exit(BossMech* mech)override;
private:
	float idleAnimationT_ = 0.0f;
};