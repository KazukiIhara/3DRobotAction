#pragma once

#include <memory>

#include "GameObject/Boss/AI/BossAI.h"
#include "GameObject/Boss/BossMech/BossMech.h"

class AttackObjectManager;
class GameEffectManager;
class MechCore;

// 実装メモ
/*
	AIクラスが機体を動かす　Enemyクラスを参考にする
*/

/// <summary>
/// ボス
/// </summary>
class Boss {
public:
	Boss(AttackObjectManager* attackObjectManager,
		GameEffectManager* gameEffectManager,
		MechCore* playerMech);

	~Boss() = default;

	void Update();
	void Draw();

	BossMech* GetMech();

	bool GetIsAIActive()const;

	void SetIsAIActive(bool isActive);

private:
	// 機体
	std::unique_ptr<BossMech> mech_;
	// AI
	std::unique_ptr<BossAI> ai_;

	// AI更新フラグ
	bool isAIActive_ = false;
};