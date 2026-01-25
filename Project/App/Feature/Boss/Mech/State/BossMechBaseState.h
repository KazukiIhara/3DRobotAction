#pragma once

#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

// ボス機体
class BossMech;

/// <summary>
///	ボス機体ステート基底クラス
/// </summary>
class BossMechBaseState {
public:
	virtual ~BossMechBaseState() = default;
	virtual void Enter(BossMech* mech) = 0;
	virtual void Update(BossMech* mech) = 0;
	virtual void Exit(BossMech* mech) = 0;
};


/// <summary>
/// コピペ用
/// </summary>
namespace {

	class PhaseName final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {}

		void Update([[maybe_unused]] BossMech* mech) override {}

		void Exit([[maybe_unused]] BossMech* mech) override {}

		bool EndRequest() override {
			return end_;
		}

	private:
		bool end_ = false;
	};

}
