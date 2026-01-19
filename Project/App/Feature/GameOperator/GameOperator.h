#pragma once

#include "Math/Types/AllMathTypes.h"

/// <summary>
/// ゲーム操作クラス
/// </summary>
class GameOperator {
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
	};
public:
	GameOperator();
	~GameOperator() = default;


};