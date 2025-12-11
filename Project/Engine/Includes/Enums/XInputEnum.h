#pragma once

#include <XInput.h>

/// <summary>
/// パッドのボタン
/// </summary>
enum PadNumber {
	ButtonA = XINPUT_GAMEPAD_A,
	ButtonB = XINPUT_GAMEPAD_B,
	ButtonX = XINPUT_GAMEPAD_X,
	ButtonY = XINPUT_GAMEPAD_Y,
	ButtonL = XINPUT_GAMEPAD_LEFT_SHOULDER,
	ButtonR = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	ButtonBack = XINPUT_GAMEPAD_BACK,
	ButtonStart = XINPUT_GAMEPAD_START,
	ButtonLeftStick = XINPUT_GAMEPAD_LEFT_THUMB,
	ButtonRightStick = XINPUT_GAMEPAD_RIGHT_THUMB,
};