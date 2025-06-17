#pragma once

// C++
#include <memory>

// MyHedder
#include "GameObject3D/GameObject3D.h"
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

	std::weak_ptr<GameObject3D> GetGameObject();

private:

	std::weak_ptr<GameObject3D> gameObject_;
	// 破壊時のエフェクト
	std::unique_ptr<BreakEffect> breakEffect_ = nullptr;

	Vector3 velocity_ = { 0.0f,0.0f };
	float speed_ = 2.0f;

	// デバッグ用変数
	bool isBreak_ = false;
	Vector4 color_ = Color::White;
};