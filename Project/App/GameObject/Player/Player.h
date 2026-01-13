#pragma once

// C++
#include <memory>

// MyHeader
#include "GameObject/Mech/MechCore/MechCore.h"
#include "Structs/ColorStruct.h"

// 前方宣言
class AttackObjectManager;
class GameEffectManger;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player {
public:
	Player(AttackObjectManager* bulletManager, GameEffectManager* effectManager);
	~Player() = default;

	void Update();

	void Draw();

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

	// 操作可能フラグをセット
	void SetIsOperation(bool isOperation);

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// プレイヤー操作可能フラグ
	bool isOperation_ = false;

	// デバッグ用変数
	bool isBreak_ = false;
	Vector4 color_ = Color::White;
};