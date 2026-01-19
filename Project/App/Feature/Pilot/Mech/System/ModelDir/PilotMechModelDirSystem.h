#pragma once

class PilotMech;

// math
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 機体の向きを制御する
/// </summary>
class PilotMechModelDirSystem {
public:
	PilotMechModelDirSystem(PilotMech* mech);
	~PilotMechModelDirSystem() = default;

	void Update();

private:
	// 移動方向にモデルを向ける
	void TurnToDirection();

private:
	PilotMech* mech_;
};