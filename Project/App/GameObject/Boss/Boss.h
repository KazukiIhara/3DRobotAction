#pragma once

#include <memory>

#include "GameObject/Boss/AI/BossAI.h"
#include "GameObject/Boss/Mech/BossMech.h"

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
	struct Flag {
		bool isPause = false;
		bool isAIActive = false;
		bool isDebugDraw = false;
	};
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

	void SwitchDebugDraw();
	void SwitchAIActive();
	void SwitchIsPause();

private:
	// 機体
	std::unique_ptr<BossMech> mech_;
	// AI
	std::unique_ptr<BossAI> ai_;

	// フラグ構造体
	Flag flag_;

};