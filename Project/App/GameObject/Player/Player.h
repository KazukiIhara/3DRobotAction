#pragma once

// C++
#include <memory>

// MyHedder
#include "Effects/BreakEffect/BreakEffect.h"
#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player {
public:
	Player();
	~Player() = default;

	void Update();

	void Draw();

	// 機体を取得
	MechCore* GetMechCore();

private:
	// 機体クラス
	std::unique_ptr<MechCore> mech_ = nullptr;

	// 破壊時のエフェクト
	std::unique_ptr<BreakEffect> breakEffect_ = nullptr;

	// デバッグ用変数
	bool isBreak_ = false;
	Vector4 color_ = Color::White;
};