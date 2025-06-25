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
	void Idle();
	// 移動
	void Move(MechCore* mechCore);
	// クイックブースト
	void StartQuickBoost(MechCore* mechCore);

	// ジャンプ
	void Jump(MechCore* mechCore);

	// 接地判定
	void CheckOnGround(MechCore* mechCore);
	// 重力計算
	void CulGravityVelocity();

	// 移動速度セット
	void SetMoveSpeed(float moveSpeed);
private:
	// 待機状態の摩擦減速
	void ApplyIdleFriction(float& v, float decelPerSec, float dt);
private:
	// 重力加速度
	const float kGravityAcc_ = -9.8f;

	// 待機時の減速度
	const float kIdleFriction_ = 30.0f;

	// 通常移動の最大速度
	const float kMaxMoveSpeed_ = 15.0f;
	// 通常移動の秒間加速量
	const float kMoveAcc_ = 30.0f;


	// クイックブーストの初速度
	const float kQuickBoostFirstSpeed_ = 30.0f;
	// クイックブーストの減速度
	const float kQuickBoostFriction_ = 30.0f;


	// ジャンプの初速度
	const float kJumpFirstSpeed_ = 10.0f;
	// ジャンプの最大速度
	const float kMaxJumpSpeed_ = 20.0f;
	// ジャンプの秒間加速量
	const float kJumpAcc_ = 30.0f;


	// 移動量
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };
	// クイックブーストの方向
	Vector2 quickBoostDirection_ = { 0.0f,0.0f };
	// 移動速度
	float moveSpeed_ = 0.0f;
	// 接地フラグ
	bool onGround_ = false;
};