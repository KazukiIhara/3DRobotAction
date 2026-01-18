#pragma once

#include <memory>

#include "GameObject/Boss/AI/BossAI.h"
#include "GameObject/Boss/Mech/BossMech.h"

// 前方宣言
class DamageObjectManager;
class GameEffectManager;
class MechAnimationContainer;
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
	Boss(DamageObjectManager* damageObjectManager,
		GameEffectManager* gameEffectManager,
		MechAnimationContainer* animationContainer,
		MechCore* playerMech);

	~Boss() = default;

	void Update();
	void Draw();

	BossMech* GetMech();

	void InitMechInitParam();

	bool GetIsAIActive()const;

	void SetIsAIActive(bool isActive);

	void SwitchDebugDraw();
	void SwitchAIActive();
	void SwitchIsPause();

private:
	// 機体
	std::unique_ptr<BossMech> mech_;
	BossMech::InitParam initParam_;

	// AI
	std::unique_ptr<BossAI> ai_;

	// フラグ構造体
	Flag flag_;

};