#pragma once

#include <iostream>

#include "Math/Types/AllMathTypes.h"

// 実装メモ
/*
	コマンドのfirstは操作可能状態かどうか
	例えばポーズ中はfirstがfalseにし、pilot側で受け取るときに操作を切れるようにする

	最終的にBattleSceneもステートで実装　そのステートに応じてどのコマンドをtrueにするのか判別するようにする
	キーコンは余裕ができたら実装する
*/

/// <summary>
/// ゲーム入力管理クラス
/// </summary>
class GameInputSystem {
public:
	struct CommonCommand {
		Vector2 StickL{};
		Vector2 StickR{};
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
	};
	struct PilotCommand {
		CommonCommand common;
		bool dodge = false;
		bool jump = false;
		bool attackL = false;
		bool attackR = false;
	};
	struct PauseCommand {
		CommonCommand common;
		bool enter = false;
		bool back = false;
	};
public:
	GameInputSystem();
	~GameInputSystem() = default;

	void Update();

	std::pair<bool, PilotCommand> GetPilotCommand() const;
private:
	void UpdatePilotCommand();
private:
	// プレイヤー操作コマンド
	std::pair<bool, PilotCommand> pilotCommand_;

};