#pragma once

// C++
#include <memory>

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public:
	Enemy();
	~Enemy() = default;

	void Update();

	// 機体を取得
	MechCore* GetMechCore();

private:
	// 機体クラス
	std::unique_ptr<MechCore> mech_ = nullptr;

};