#pragma once

#include "Feature/Boss/Mech/State/BossMechBaseState.h"

#include <memory>

#include "Feature/Boss/Mech/State/PhaseSys/Runner/BossMechStatePhaseRunner.h"

/// <summary>
/// レーザー攻撃
/// </summary>
class BossMechStateLaserShot : public BossMechBaseState {
public:
	~BossMechStateLaserShot() override = default;

	void Enter(BossMech* mech) override;
	void Update(BossMech* mech) override;
	void Exit(BossMech* mech) override;

private:
	// Phase生成登録
	void SetupPhases();

private:
	std::unique_ptr<BossMechStatePhaseRunner> runner_;
};
