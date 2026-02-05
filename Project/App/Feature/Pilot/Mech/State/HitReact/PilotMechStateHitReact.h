#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

/// <summary>
/// 中衝撃の攻撃を受けた時のリアクションステート
/// </summary>
class PilotMechStateHitReact :public IPilotMechState {
public:
	~PilotMechStateHitReact()override = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;
private:
	float timer_ = 0.0f;

};