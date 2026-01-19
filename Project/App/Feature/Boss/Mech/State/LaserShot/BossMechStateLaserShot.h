#pragma once

#include "Feature/Boss/Mech/State/BossMechBaseState.h"

// 実装メモ
/*
	レーザー攻撃ステート
	機体アニメーション再生　→　射撃　まで
*/

/// <summary>
/// レーザー攻撃
/// </summary>
class BossMechStateLaserShot:public BossMechBaseState {
public:
	~BossMechStateLaserShot()override = default;
	void Enter(BossMech* mech)override;
	void Update(BossMech* mech)override;
	void Exit(BossMech* mech)override;
private:
	// タイマー
	float time_ = 0.0f;
};