#pragma once

// C++
#include <memory>

// MyHedder
#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/PlayerUI/PlayerUI.h"
#include "Structs/ColorStruct.h"

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

	void SetBossMech(std::weak_ptr<MechCore> bossMechCore);

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// プレイヤーUIクラス
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;


	// デバッグ用変数
	bool isBreak_ = false;
	Vector4 color_ = Color::White;
};