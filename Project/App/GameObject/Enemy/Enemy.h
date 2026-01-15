#pragma once

// C++
#include <memory>

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/EnemyAI/EnemyAI.h"

// 前方宣言
class AttackObjectManager;
class GameEffectManager;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public:
	Enemy(AttackObjectManager* bulletManager, GameEffectManager* effectManager, MechCore* playerMech);
	~Enemy() = default;

	void Update();

	void Draw();

	// 機体を取得
	MechCore* GetMechCore();

	bool GetIsAIActive() const;

	void SetIsAIActive(bool isActive);

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// 敵AIクラス
	std::unique_ptr<EnemyAI> ai_;

	// AIのアクティブフラグ
	bool isAIActive_ = true;
};