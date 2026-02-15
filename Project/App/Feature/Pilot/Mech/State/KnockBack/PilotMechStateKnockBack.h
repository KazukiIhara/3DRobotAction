#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

class PilotMechStateKnockBack :public IPilotMechState {
public:
	~PilotMechStateKnockBack()override = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;
private:
	float timer_ = 0.0f;
	
};