#pragma once

#define NOMINMAX

#include <Windows.h>
#include "Enums/XInputEnum.h"
#include "Structs/XInputStruct.h"
#include "Math/Types/Vector2.h"

// 前方宣言
class DeltaTimer;

/// <summary>
/// Xインプットクラス
/// </summary>
class MAGIXInput {
public:
	MAGIXInput(DeltaTimer* deltaTimer);
	~MAGIXInput();

	// 更新
	void Update();

	// パッドの接続状況を確認
	bool IsConnected(int controllerID) const;

	// コントローラのボタン入力
	bool PushButton(int controllerID, int buttonNumber) const;
	bool TriggerButton(int controllerID, int buttonNumber) const;
	bool HoldButton(int controllerID, int buttonNumber) const;
	bool ReleaseButton(int controllerID, int buttonNumber) const;

	// スティックやトリガーの位置取得
	Vector2 GetLeftStick(int controllerID)const;
	Vector2 GetRightStick(int controllerID)const;

	float GetLeftTrigger(int controllerID) const;
	float GetRightTrigger(int controllerID) const;


	// 方向キー
	bool IsPadUp(int controllerID) const;
	bool IsPadRight(int controllerID) const;
	bool IsPadDown(int controllerID) const;
	bool IsPadLeft(int controllerID) const;

	// デッドゾーンの設定
	void SetDeadZone(int deadZone);
	// デッドゾーンの取得
	int GetDeadZone() const;

	// 振動開始
	void StartVibration(int controllerID, float duration, float leftPower, float rightPower);

	// 振動停止
	void StopVibration(int controllerID);

private:
	float ProcessDeadZone(float value) const;
	Vector2 ProcessRadialDeadZone(float rawX, float rawY) const;
private:
	// 最大4つのコントローラの状態を管理
	XINPUT_STATE gamepadStates[4]{};
	XINPUT_STATE preGamepadStates[4]{};
	// 各パッドの振動状態を管理
	VibrationState vibrationStates[4];

	// スティックの遊び
	int deadZone_ = 30;

private:
	DeltaTimer* deltaTimer_ = nullptr;
};