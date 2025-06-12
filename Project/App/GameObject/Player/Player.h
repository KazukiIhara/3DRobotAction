#pragma once

// C++
#include <memory>

// MyHedder
#include "GameObject3D/GameObject3D.h"
#include "Effects/BreakEffect/BreakEffect.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player :public GameObject3D {
public:
	Player(const std::string& name);
	~Player() = default;

	void Update()override;

	void Draw();

private:

	// 破壊時のエフェクト
	std::unique_ptr<BreakEffect> breakEffect_ = nullptr;

	Vector3 velocity_;
	float speed_ = 2.0f;

	// デバッグ用変数
	bool isBreak_ = false;
	Vector4 color_ = Color::White;
};