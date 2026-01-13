#pragma once

#include <memory>

#include "GameObject/Boss/AI/BossAI.h"
#include "GameObject/Boss/BossMech/BossMech.h"

// 実装メモ
/*
	AIクラスが機体を動かす　Enemyクラスを参考にする

*/

/// <summary>
/// ボス
/// </summary>
class Boss {
public:
	Boss();
	~Boss() = default;

private:
	// 機体
	std::unique_ptr<BossMech> mech_;
	// AI
	std::unique_ptr<BossAI> ai_;
};