#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

/// <summary>
/// 移動ステート
/// </summary>
class PilotMechStateMove:public IPilotMechState {
public:
	~PilotMechStateMove()override = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;

};