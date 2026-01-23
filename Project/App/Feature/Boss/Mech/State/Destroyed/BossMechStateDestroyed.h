#pragma once

#include "Feature/Boss/Mech/State/BossMechBaseState.h"

// 実装メモ

/*
	撃破ステート
	戦闘シーン終了フェーズ中のステート
*/

/// <summary>
/// ボス機体破壊クラス
/// </summary>
class BossMechStateDestroyed:public BossMechBaseState {
public:
	~BossMechStateDestroyed() = default;
	void Enter(BossMech* mech)override;
	void Update(BossMech* mech)override;
	void Exit(BossMech* mech)override;
};