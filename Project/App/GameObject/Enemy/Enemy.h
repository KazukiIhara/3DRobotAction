#pragma once

// C++
#include <memory>

#include "GameObject/Mech/MechCore/MechCore.h"

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public:
	Enemy();
	~Enemy() = default;

	void Update();

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

};