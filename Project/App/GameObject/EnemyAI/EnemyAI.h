#pragma once

// C++
#include <memory>
#include <unordered_map>

// アプリ用汎用ヘッダ
#include "GameCommon/GameCommon.h"
#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"

// 前方宣言
class MechCore;

/// <summary>
/// 敵AIの状態
/// </summary>
enum class EnemyAIState {

};

/// <summary>
/// 敵のAIクラス
/// </summary>
class EnemyAI {
public:
	EnemyAI(std::weak_ptr<MechCore>playerMech);
	~EnemyAI() = default;

	InputCommand Update(MechCore* mechCore);

private:
	// プレイヤーの機体のポインタ
	std::weak_ptr<MechCore> playerMech_;

	// ステートテーブル
	std::unordered_map<EnemyAIState, std::shared_ptr<BaseEnemyAIState>> state_;
	// 現在のステート
	std::pair<EnemyAIState, std::weak_ptr<BaseEnemyAIState>> currentState_;

	// 冷静さ
	int32_t calmness_ = 3;
	
};