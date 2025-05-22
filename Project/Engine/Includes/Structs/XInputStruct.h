#pragma once

#include <Xinput.h>

/// <summary>
/// パッド振動用構造体
/// </summary>
struct VibrationState {
	bool isActive = false;
	float timer = 0.0f;
	float duration = 0.0f;
	WORD leftMotor = 0;
	WORD rightMotor = 0;
};