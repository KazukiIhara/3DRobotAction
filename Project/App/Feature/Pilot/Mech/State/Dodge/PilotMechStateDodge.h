#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

/// <summary>
/// 回避
/// </summary>
class PilotMechStateDodge:public IPilotMechState {
public:
	~PilotMechStateDodge() = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;
private:
	float timer_ = 0.0f;
};