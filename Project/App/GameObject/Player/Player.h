#pragma once

// C++
#include <memory>

// MyHedder
#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/PlayerUI/PlayerUI.h"
#include "Structs/ColorStruct.h"

// 前方宣言
class AttackObjectManager;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player {
public:
	Player(AttackObjectManager* bulletManager);
	~Player() = default;

	void Update();

	void Draw();

	void SetBossMech(std::weak_ptr<MechCore> bossMechCore);

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

	// 操作可能フラグをセット
	void SetIsOperation(bool isOperation);

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// プレイヤーUIクラス
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;

	// プレイヤー操作可能フラグ
	bool isOperation_ = false;

	// デバッグ用変数
	bool isBreak_ = false;
	magi::Vector4 color_ = Color::White;
};