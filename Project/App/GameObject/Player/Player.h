#pragma once

// C++
#include <memory>

// MyHedder
#include "Transform3D/Transform3D.h"


#include "Effects/BreakEffect/BreakEffect.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player {
public:
	Player();
	~Player() = default;

	void Update();

	void Draw();

private:
	// トランスフォーム
	std::unique_ptr<Transform3D> transform_;
	// 描画用トランスフォーム
	std::unique_ptr<Transform3D> renderTransform_;

	// 破壊時のエフェクト
	std::unique_ptr<BreakEffect> breakEffect_ = nullptr;

	// デバッグ用変数
	bool isBreak_ = false;
};