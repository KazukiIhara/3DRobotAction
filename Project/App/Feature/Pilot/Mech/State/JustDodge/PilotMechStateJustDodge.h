#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

/// <summary>
/// ジャスト回避
/// </summary>
class PilotMechStateJustDodge:public IPilotMechState {
public:
	~PilotMechStateJustDodge() = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;

};