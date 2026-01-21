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
class BossMechStateLaserShot :public BossMechBaseState {
public:
	enum class Phase {
		StartUp,
		Charge,
		EndLag,
	};
public:
	~BossMechStateLaserShot()override = default;
	void Enter(BossMech* mech)override;
	void Update(BossMech* mech)override;
	void Exit(BossMech* mech)override;

private:
	void UpdateStartUp(float dt, BossMech* mech);
	void UpdateCharge(float dt, BossMech* mech);
	void UpdateEndLag(float dt, BossMech* mech);
private:
	// フェーズ
	Phase phase_ = Phase::StartUp;
	// タイマー
	float timer_ = 0.0f;
};