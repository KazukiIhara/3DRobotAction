#pragma once

// C++
#include <memory>

// MyHedder
#include "Effects/BreakEffect/BreakEffect.h"
#include "GameObject/Mech/MechCore/MechCore.h"

// 前方宣言
class BulletManager;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player {
public:
	Player(BulletManager* bulletManager);
	~Player() = default;

	void Update();

	void Draw();

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// 破壊時のエフェクト
	std::unique_ptr<BreakEffect> breakEffect_ = nullptr;

	// デバッグ用変数
	bool isBreak_ = false;
	Vector4 color_ = Color::White;
};