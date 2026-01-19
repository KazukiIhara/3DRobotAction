#pragma once

class PilotMech;

// math
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 機体を動かすクラス
/// </summary>
class PilotMechMoveSystem {
public:
	PilotMechMoveSystem(PilotMech* mech);
	~PilotMechMoveSystem() = default;

	// 機体を動かす
	void Update();

	void SetDir(const Vector3& dir);
	void SetSpeed(float speed);
	void SetMaxSpeed(float maxSpeed);

	const Vector3 GetVelocity()const;

private:
	Vector3 velocity_;
	Vector3 dir_;
	float speed_;
	float maxSpeed_;

	PilotMech* mech_;
};