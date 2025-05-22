#include "MAGIXInput.h"

#include <cassert>
#include "math.h"

#include "DeltaTimer/DeltaTimer.h"

MAGIXInput::MAGIXInput(DeltaTimer* deltaTimer) {
	assert(deltaTimer);
	deltaTimer_ = deltaTimer;
}

MAGIXInput::~MAGIXInput() {}

void MAGIXInput::Update() {
	for (int i = 0; i < 4; ++i) {
		preGamepadStates[i] = gamepadStates[i];
		DWORD dwResult = XInputGetState(i, &gamepadStates[i]);
		if (dwResult != ERROR_SUCCESS) {
			ZeroMemory(&gamepadStates[i], sizeof(XINPUT_STATE));
		}

		// 振動処理
		if (vibrationStates[i].isActive && vibrationStates[i].duration > 0.0f) {
			vibrationStates[i].timer += deltaTimer_->GetDeltaTime();

			if (vibrationStates[i].timer >= vibrationStates[i].duration) {
				StopVibration(i);
			}
		}

	}
}

bool MAGIXInput::IsConnected(int controllerID) const {
	if (controllerID < 0 || controllerID >= 4) {
		return false; // 範囲外のIDなら接続されていないと判断
	}
	XINPUT_STATE state;
	DWORD dwResult = XInputGetState(controllerID, &state);
	return (dwResult == ERROR_SUCCESS);
}

bool MAGIXInput::PushButton(int controllerID, int buttonNumber) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & buttonNumber) != 0;
}

bool MAGIXInput::TriggerButton(int controllerID, int buttonNumber) const {
	return !(preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

bool MAGIXInput::HoldButton(int controllerID, int buttonNumber) const {
	return (preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

bool MAGIXInput::ReleaseButton(int controllerID, int buttonNumber) const {
	return (preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		!(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

float MAGIXInput::GetLeftStickX(int controllerID) const {
	float rawValue = gamepadStates[controllerID].Gamepad.sThumbLX / 32767.0f;
	return ProcessDeadZone(rawValue);
}

float MAGIXInput::GetLeftStickY(int controllerID) const {
	float rawValue = gamepadStates[controllerID].Gamepad.sThumbLY / 32767.0f;
	return ProcessDeadZone(rawValue);
}

float MAGIXInput::GetRightStickX(int controllerID) const {
	float rawValue = gamepadStates[controllerID].Gamepad.sThumbRX / 32767.0f;
	return ProcessDeadZone(rawValue);
}

float MAGIXInput::GetRightStickY(int controllerID) const {
	float rawValue = gamepadStates[controllerID].Gamepad.sThumbRY / 32767.0f;
	return ProcessDeadZone(rawValue);
}

float MAGIXInput::GetLeftTrigger(int controllerID) const {
	return gamepadStates[controllerID].Gamepad.bLeftTrigger / 255.0f;
}

float MAGIXInput::GetRightTrigger(int controllerID) const {
	return gamepadStates[controllerID].Gamepad.bRightTrigger / 255.0f;
}

bool MAGIXInput::IsPadUp(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
}

bool MAGIXInput::IsPadRight(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
}

bool MAGIXInput::IsPadDown(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
}

bool MAGIXInput::IsPadLeft(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
}

void MAGIXInput::SetDeadZone(int deadZone) {
	deadZone_ = (deadZone < 0) ? 0 : (deadZone > 100) ? 100 : deadZone;
}

int MAGIXInput::GetDeadZone() const {
	return deadZone_;
}

void MAGIXInput::StartVibration(int controllerID, float duration, float leftPower, float rightPower) {
	if (controllerID < 0 || controllerID >= 4) return;

	WORD left = static_cast<WORD>(leftPower * 65535);
	WORD right = static_cast<WORD>(rightPower * 65535);

	XINPUT_VIBRATION vibration = {};
	vibration.wLeftMotorSpeed = left;
	vibration.wRightMotorSpeed = right;
	XInputSetState(controllerID, &vibration);

	vibrationStates[controllerID].isActive = true;
	vibrationStates[controllerID].timer = 0.0f;
	vibrationStates[controllerID].duration = duration;
	vibrationStates[controllerID].leftMotor = left;
	vibrationStates[controllerID].rightMotor = right;
}

void MAGIXInput::StopVibration(int controllerID) {
	if (controllerID < 0 || controllerID >= 4) return;

	XINPUT_VIBRATION vibration = {};
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(controllerID, &vibration);

	vibrationStates[controllerID] = VibrationState(); // 初期化
}

float MAGIXInput::ProcessDeadZone(float value) const {
	float maxValue = 1.0f; // スティックの最大値
	float threshold = maxValue * (deadZone_ / 100.0f); // デッドゾーンを割合で計算

	if (fabs(value) < threshold) {
		return 0.0f; // デッドゾーン内の値は0にする
	} else {
		// デッドゾーンを超えたら、リマップ
		return (value > 0) ? (value - threshold) / (maxValue - threshold)
			: (value + threshold) / (maxValue - threshold);
	}
}