#pragma once

// 数学ヘッダ
#include "Math/Utility/MathUtility.h"

/// <summary>
/// 識別タグ
/// </summary>
enum class FriendlyTag {
	PlayerSide,
	EnemySide,
};

/// <summary>
/// 攻撃の種類
/// </summary>
enum class AttackType {
	Bullet,
	Melee,
};

/// <summary>
/// ロックオンカメラ
/// </summary>
struct LockOnView {
	// カメラの位置
	Vector3 eye = { 0.0f,0.0f,0.0f };
	// カメラのターゲット
	Vector3 target = { 0.0f,0.0f,0.0f };
	// 上方向
	Vector3 up = { 0.0f,1.0f,0.0f };
	// ニアクリップ距離
	float nearClipRange = 0.1f;
	// ファークリップ距離
	float farClipRange = 100.0f;
};

/// <summary>
/// 入力コマンド
/// </summary>
struct InputCommand {
	Vector2 moveDirection = { 0.0f,0.0f };
	Vector2 cameraRotDirection = { 0.0f,0.0f };
	bool jump = false;
	bool quickBoost = false;
	bool assultBoost = false;
	bool switchHardLock = false;
	bool leftHandWeapon = false;
	bool rightHandWeapon = false;
};