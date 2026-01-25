#pragma once

#include "Feature/Boss/Mech/State/BossMechBaseState.h"

// 実装メモ

/*
	平行軸接近からの2連斬撃
*/

/// <summary>
/// レーザー近接攻撃
/// </summary>
class BossMechStateLaserBladeSlash:public BossMechBaseState {
public:
	enum class Phase {
		Approach,
		Attack1,
		Attack2,
		EndLag,
	};
public:
	~BossMechStateLaserBladeSlash() = default;
	void Enter(BossMech* mech)override;
	void Update(BossMech* mech)override;
	void Exit(BossMech* mech)override;

private:
	// タイマー
	float timer_ = 0.0f;
};