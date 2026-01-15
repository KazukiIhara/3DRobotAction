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

	void SwitchDebugDraw();
	void SwitchAIActive();


	bool GetIsAIActive()const;
	void SetIsAIActive(bool isActive);

private:
	// 機体
	std::unique_ptr<BossMech> mech_;
	// AI
	std::unique_ptr<BossAI> ai_;

	// フラグ構造体
	Flag flag_;

};