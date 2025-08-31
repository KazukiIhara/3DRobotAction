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
	Approach,	// 接近
	Avoid,		// 回避
	Leave,		// 離脱
	Escape,		// 逃走
};

enum class RootDir {
	Left,
	Right,
};

/// <summary>
/// 敵のAIクラス
/// </summary>
class EnemyAI {
public:
	EnemyAI(std::weak_ptr<MechCore> mechCore, std::weak_ptr<MechCore>playerMech);
	~EnemyAI() = default;

	InputCommand Update();
	void ChangeState(EnemyAIState nextState);

	// 自機を取得
	MechCore* GetPlayerMech();

	// 現在の旋回方向を取得
	RootDir GetRootDir() const;

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

	// ステートテーブル
	std::unordered_map<EnemyAIState, std::shared_ptr<BaseEnemyAIState>> states_;
	// 現在のステート
	std::pair<EnemyAIState, std::weak_ptr<BaseEnemyAIState>> currentState_;

	// 冷静さ
	int32_t calmness_ = 3;

	// ステート変更タイマー
	float stateChangeTimer_ = 0.0f;
	// ステート変更時間
	float stateChangeTime_ = 1.0f;

	// 現在の旋回方向
	RootDir rootDir_ = RootDir::Left;
};