#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

/// <summary>
/// 待機ステート
/// </summary>
class PilotMechStateIdle:public IPilotMechState {
public:
	~PilotMechStateIdle()override = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;
private:
	bool preIsGround_ = false;
};