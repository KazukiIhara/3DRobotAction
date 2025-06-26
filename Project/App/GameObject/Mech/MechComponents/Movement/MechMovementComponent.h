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
	void QuickBoostEnter(MechCore* mechCore);
	// クイックブースト更新
	void QuickBoostUpdate();
	// クイックブースト終了通知
	bool QuickBoostEndRequest()const;

	// ジャンプ
	void Jump(MechCore* mechCore);

	// 接地判定
	void CheckOnGround(MechCore* mechCore);
	// 重力計算
	void CulGravityVelocity();

private:
	// 待機状態の摩擦減速
	void ApplyIdleFriction(float& v, float decelPerSec, float dt);

private:
	//===================================
	// 移動用パラメータ
	//===================================

	// 重力加速度
	const float kGravityAcc_ = -9.8f;

	// 接地時の減速度
	const float kIdleFrictionGround_ = 30.0f;
	// 空中時の減速度
	const float kIdleFrictionAir_ = 15.0f;

	// 通常移動の最大速度
	const float kMaxMoveSpeed_ = 15.0f;
	// 通常移動の秒間加速量
	const float kMoveAcc_ = 30.0f;


	// クイックブーストの初速度
	const float kQuickBoostFirstSpeed_ = 30.0f;
	// クイックブーストの減速度
	const float kQuickBoostFriction_ = 30.0f;
	// クイックブーストのリロード時間
	const float kQuickBoostTime_ = 0.35f;


	// ジャンプの初速度
	const float kJumpFirstSpeed_ = 10.0f;
	// ジャンプの最大速度
	const float kMaxJumpSpeed_ = 20.0f;
	// ジャンプの秒間加速量
	const float kJumpAcc_ = 30.0f;


	//===================================
	// 変数群
	//===================================

	// 進行方向
	Vector2 currentMoveDir_ = { 0.0f,0.0f };
	// スピード
	float moveSpeed_ = 0.0f;
	// 移動量
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };


	// クイックブースト
	Vector2 quickBoostDir_ = { 0.0f,0.0f };
	// クイックブーストのタイマー
	float quickBoostTimer_ = 0.0f;

	// 接地フラグ
	bool onGround_ = false;
};