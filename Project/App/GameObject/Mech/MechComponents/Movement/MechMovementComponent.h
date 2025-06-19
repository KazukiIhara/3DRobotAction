#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"

class MechCore;

/// <summary>
/// 移動コンポーネント
/// </summary>
class MechMovementComponent {
public:
	MechMovementComponent() = default;
	~MechMovementComponent() = default;

	// 更新
	void Update(MechCore* mechCore);

	// 待機
	void Idle(MechCore* mechCore);
	// 移動
	void Move(MechCore* mechCore);

	// 接地判定
	bool CheckOnGround(MechCore* mechCore);

	// 移動速度セット
	void SetMoveSpeed(float moveSpeed);

private:
	// 重力加速度
	const float kGravityAcc_ = -9.8f;
	// 通常移動の最大速度
	const float kMaxMoveSpeed_ = 15.0f;
	// 通常移動の秒間加速量
	const float kMoveAcc_ = 30.0f;


	// 移動量
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };
	// 移動速度
	float moveSpeed_ = 0.0f;
	// 接地フラグ
	bool onGround_ = false;
};