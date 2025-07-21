#pragma once

// C++
#include <memory>

// アプリ用汎用ヘッダ
#include "GameCommon/GameCommon.h"

// 前方宣言
class MechCore;

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
};