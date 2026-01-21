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
	void SetDirXZ(const Vector2& dirXZ);
	void SetAcc(float acc);
	void SetSpeed(float speed);
	void SetMaxSpeed(float maxSpeed);

	float GetSpeed()const;
	const Vector3& GetDir()const;
	const Vector3& GetVelocity()const;

private:
	// 旋回による減速処理
	void TurnDeceleration();

private:
	Vector3 velocity_;
	Vector3 dir_;
	Vector3 preDir_;
	float acc_;
	float speed_;
	float maxSpeed_;

	PilotMech* mech_;
};