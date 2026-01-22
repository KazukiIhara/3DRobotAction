#pragma once

// 機体
class BaseMech;

// math
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 機体移動システム
/// </summary>
class MechMoveSystem {
public:
	MechMoveSystem(BaseMech* mech);
	~MechMoveSystem() = default;

	// 移動量を計算
	void CalSpeed();

	// 移動量を追加
	void ApplyVelocity();

	void SetDir(const Vector3& dir);
	void SetDirXZ(const Vector2& dirXZ);
	void SetAcc(float acc);
	void SetSpeed(float speed);
	void SetMaxSpeed(float maxSpeed);

	float GetSpeed()const;
	const Vector3& GetDir()const;
	const Vector3& GetVelocity()const;

	// 回転を補間でゆっくりにする
	void ApplyRotationInertia();

private:
	Vector3 velocity_;
	Vector3 dir_;
	Vector3 preDir_;
	float acc_;
	float speed_;
	float maxSpeed_;

	BaseMech* mech_;
};