#pragma once

// C++
#include <memory>

#include "GameObject/Mech/MechCore/MechCore.h"

// 前方宣言
class BulletManager;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public:
	Enemy(BulletManager* bulletManager);
	~Enemy() = default;

	void Update();

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

};