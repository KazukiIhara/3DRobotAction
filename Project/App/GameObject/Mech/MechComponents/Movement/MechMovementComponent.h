#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "SimpleAnimation/SimpleAnimation.h"

// 前方宣言
class MechCore;

/// <summary>
/// 移動コンポーネント
/// </summary>
class MechMovementComponent {
public:
	MechMovementComponent();
	~MechMovementComponent() = default;

	// 更新
	void Update(MechCore* mechCore);

	// 待機
	void Idle();
	// 移動
	void Move(MechCore* mechCore);
	// クイックブースト
	void QuickBoostEnter(MechCore* mechCore);
	// アサルトブースト
	void AssultBoostEnter(MechCore* mechCore);

	// アサルトブースト更新
	void AssultBoostUpdate(MechCore* mechCore);

	// クイックブースト更新
	void QuickBoostUpdate();
	// クイックブースト終了通知
	bool QuickBoostEndRequest()const;
	// クイックブーストキャンセル可能通知
	bool QuickBoostEnableCancel()const;

	// 逆入力時減速処理
	void ReverseInputDeceleration(MechCore* mechCore);

	// ジャンプ
	void Jump(MechCore* mechCore);

	// 接地判定
	void CheckOnGround(MechCore* mechCore);

	// 重力計算
	void CulGravityVelocity(MechCore* mechCore);

	//===================================
	// ゲッター
	//===================================

	// 現在の進行方向を取得
	Vector2 GetCurrentMoveDir()const;

	// 現在の速度を取得
	float GetCurrentSpeed()const;
	// 最大速度を取得
	float GetMaxSpeed()const;

	// 現在の移動量を取得
	Vector3 GetCurrentVelocity()const;

	//===================================
	// セッター
	//===================================

	// 移動速度セット
	void SetMoveSpeed(float s);

private:
	// 待機状態の摩擦減速
	void ApplyIdleFriction(float& v, float decelPerSec, float dt);

private:
	//===================================
	// 移動用パラメータ　(外部から持ってくる構造にする)
	//===================================

	// 重力加速度
	const float kGravityAcc_ = -9.8f;
	const float kGravityScale_ = 3.0f;

	// 接地時の減速度
	const float kIdleFrictionGround_ = 30.0f;
	// 空中時の減速度
	const float kIdleFrictionAir_ = 15.0f;

	// 通常移動の最大速度
	const float kMaxMoveSpeed_ = 15.0f;

	// 通常移動の秒間加速量
	const float kMoveAcc_ = 20.0f;
	// 空中時の通常移動秒間加速量
	const float kMoveAccAir_ = 10.0f;

	// クイックブーストの初速度
	const float kQuickBoostFirstSpeed_ = 50.0f;
	// クイックブーストの時間
	const float kQuickBoostTime_ = 0.6f;
	// クイックブーストキャンセル時間
	const float kQuickBoostCancelTime_ = 0.3f;
	const Vector3 kQuickBoostCameraShakeIntensity_ = { 0.0f,0.0f,0.0f };

	// アサルトブーストの最大速度
	const float kMaxAssultBoostSpeed_ = 50.0f;

	// ジャンプの初速度
	const float kJumpFirstSpeed_ = 10.0f;
	// ジャンプの最大速度
	const float kMaxJumpSpeed_ = 10.0f;
	// ジャンプの秒間加速量
	const float kJumpAcc_ = 5.0f;


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
	// クイックブーストの移動量アニメーション
	SimpleAnimation<float> quickBoostSpeedAnimaion_;



	// 接地フラグ
	bool onGround_ = false;
};