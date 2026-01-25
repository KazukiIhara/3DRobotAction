#pragma once

#include "Feature/Boss/Mech/State/BossMechBaseState.h"

#include <memory>

#include "Feature/Boss/Mech/State/PhaseSys/Runner/BossMechStatePhaseRunner.h"

// 実装メモ

/*
	平行軸接近からの2連斬撃
*/

/// <summary>
/// レーザー近接攻撃
/// </summary>
class BossMechStateLaserBladeSlash:public BossMechBaseState {
public:
	~BossMechStateLaserBladeSlash() = default;
	void Enter(BossMech* mech)override;
	void Update(BossMech* mech)override;
	void Exit(BossMech* mech)override;
private:
	// Phase生成登録
	void SetupPhases();

private:
	std::unique_ptr<BossMechStatePhaseRunner> runner_;
};