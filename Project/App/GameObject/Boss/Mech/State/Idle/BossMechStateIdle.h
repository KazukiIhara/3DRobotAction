#pragma once

#include "GameObject/Boss/Mech/State/BossMechBaseState.h"

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
	void Enter(BossMech* mechCore);
	void Update(BossMech* mechCore);
	void Exit(BossMech* mechCore);
};