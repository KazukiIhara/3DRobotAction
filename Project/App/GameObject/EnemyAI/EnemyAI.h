#pragma once

// C++
#include <memory>
#include <unordered_map>

// アプリ用汎用ヘッダ
#include "GameCommon/GameCommon.h"


// 前方宣言
class MechCore;
class BaseEnemyAIState;

/// <summary>
/// 敵AIのステート
/// </summary>
enum class EnemyAIState {
	Root,		// 通常
	Search,		// 索敵
	Avoid,		// 回避
};

enum class RootDir {
	Left,
	Right,
};

struct AvoidColliderAABB {
	const float localMinMax = 4.0f;
	const float localMinMaxY = 8.0f;

	Vector3 min;
	Vector3 max;
};

// 弾マネージャ
class AttackObjectManager;

/// <summary>
/// 敵のAIクラス
/// </summary>
class EnemyAI {
public:
	EnemyAI(std::weak_ptr<MechCore> mechCore, std::weak_ptr<MechCore>playerMech, AttackObjectManager* bulletManager);
	~EnemyAI() = default;

	InputCommand Update();
	void ChangeState(EnemyAIState nextState);

	// 自機を取得
	MechCore* GetPlayerMech();

	// 現在の旋回方向を取得
	RootDir GetRootDir() const;

	// 回避用のコライダーを取得
	AvoidColliderAABB GetAvoidCollider() const;

	//
	// 各ステートからコマンドを入力する際に呼ぶ関数
	//
	void MoveDir(const Vector2& dir);
	void CameraRotDir(const Vector2& camRDir);
	void Jump();
	void QuickBoost();
	void AssultBoost();
	void LeftHandWeapon();
	void RightHandWeapon();
	void SetRootDir(RootDir dir);

	// 弾マネージャを取得
	AttackObjectManager* GetBulletManager();

private:
	// 対応するステートを取得
	std::weak_ptr<BaseEnemyAIState> GetState(EnemyAIState state);

	// 入力された方向をカメラに対しての向きに直す
	void CulDirectionWithCamera(MechCore* mechCore);

private:
	// 自機のポインタ
	MechCore* mechCore_;

	// プレイヤーの機体のポインタ
	std::weak_ptr<MechCore> playerMech_;

	// 出力するコマンド
	InputCommand command_;
	
	// 索敵時のロックオンビュー
	LockOnView lockOnView_;

	// ステートテーブル
	std::unordered_map<EnemyAIState, std::shared_ptr<BaseEnemyAIState>> states_;
	// 現在のステート
	std::pair<EnemyAIState, std::weak_ptr<BaseEnemyAIState>> currentState_;

	// 現在の旋回方向
	RootDir rootDir_ = RootDir::Left;

	// 回避用コライダーのトランスレート
	Vector3 avoidColliderTranslate_ = { 0.0f,0.0f,8.0f };

	// 回避用コライダー
	AvoidColliderAABB avoidCollider_;

	// 弾マネージャのポインタ
	AttackObjectManager* attackObjectManager_ = nullptr;
};