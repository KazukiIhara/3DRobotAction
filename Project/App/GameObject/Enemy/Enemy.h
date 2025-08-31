#pragma once

// C++
#include <memory>

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/EnemyAI/EnemyAI.h"

// 前方宣言
class BulletManager;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public:
	Enemy(BulletManager* bulletManager, std::weak_ptr<MechCore> playerMech);
	~Enemy() = default;

	void Update();

	void SetIsAIActive(bool isActive);

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// 敵AIクラス
	std::unique_ptr<EnemyAI> ai_;

	// AIのアクティブフラグ
	bool isAIActive_ = true;
};