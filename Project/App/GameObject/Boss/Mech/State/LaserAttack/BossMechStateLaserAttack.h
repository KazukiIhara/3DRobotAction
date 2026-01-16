#pragma once

#include "GameObject/Boss/Mech/State/BossMechBaseState.h"

// 実装メモ
/*
	レーザー攻撃　アニメーション再生　→　射撃　まで
*/

/// <summary>
/// レーザー攻撃
/// </summary>
class BossMechStateLaserAttack:public BossMechBaseState {
public:
	~BossMechStateLaserAttack()override = default;
	void Enter(BossMech* mechCore)override;
	void Update(BossMech* mechCore)override;
	void Exit(BossMech* mechCore)override;

private:

};