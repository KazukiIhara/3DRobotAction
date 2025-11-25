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

struct AvoidCollider {
	// 胴体のワールド座標
	Vector3 wPos;
	// 周辺探索用の半径
	float radius;

	// カメラコライダー用データ
	Vector3 eye; // 機体の目の座標
	Vector3 target; // カメラの向いている方向 MechCoreのLockOnViewから取得
	Vector3 up = { 0.0f,1.0f,0.0f };
	float nearClip = 0.1f;
	float farClip = 10.0f;
};

// 
// 前方宣言
// 

// 弾マネージャ
class AttackObjectManager;

// 攻撃コリジョンマネージャ
class AttackCollisionManager;

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
	AvoidCollider GetAvoidCollider()const;

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
	AttackObjectManager* GetAttackObjectManager();

private:
	// 対応するステートを取得
	std::weak_ptr<BaseEnemyAIState> GetState(EnemyAIState state);

	// 入力された方向をカメラに対しての向きに直す
	void CulDirectionWithCamera(MechCore* mechCore);

	// 回避用のコライダーを更新
	void UpdateAvoidCollider(MechCore* mechCore);

private:
	// 自機のポインタ
	MechCore* mechCore_;

	// プレイヤーの機体のポインタ
	std::weak_ptr<MechCore> playerMech_;

	// 出力するコマンド
	InputCommand command_;

	// ステートテーブル
	std::unordered_map<EnemyAIState, std::shared_ptr<BaseEnemyAIState>> states_;
	// 現在のステート
	std::pair<EnemyAIState, std::weak_ptr<BaseEnemyAIState>> currentState_;

	// 現在の旋回方向
	RootDir rootDir_ = RootDir::Left;

	// 回避用コライダー
	AvoidCollider avoidCollider_;

	// 弾マネージャのポインタ
	AttackObjectManager* attackObjectManager_ = nullptr;

	// 攻撃コライダーのマネージャ
	AttackCollisionManager* attackCollisionManager_ = nullptr;

	// 
	// パラメータ
	// 

	// 機体周辺探索用の半径
	float avoidRadius_ = 3.0f;
};