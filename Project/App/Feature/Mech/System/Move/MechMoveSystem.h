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

	void Update();

	void SetDir(const Vector3& dir);
	void SetDirY(float dirY);
	void SetDirXZ(const Vector2& dirXZ);
	void SetAcc(float acc);
	void SetSpeed(float speed);
	void SetMaxSpeed(float maxSpeed);

	float GetSpeed()const;
	const Vector3& GetDir()const;
	const Vector3& GetVelocity()const;

	void SetVerticalSpeed(float vertical);
	float GetVerticalSpeed() const;

	void Reset();

	// 回転を補間でゆっくりにする
	void ApplyRotationInertia();
private:
	// 移動量を計算
	void CalSpeed();

	// 移動量を追加
	void ApplyVelocity();
private:
	Vector3 velocity_;
	Vector3 dir_;
	Vector3 preDir_;
	float acc_;
	float speed_;
	float maxSpeed_;

	float verticalSpeed_;

	BaseMech* mech_;
};