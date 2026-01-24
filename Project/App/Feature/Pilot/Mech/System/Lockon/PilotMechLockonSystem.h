#pragma once

// ロックオン対象のオブジェクトのインターフェース
#include "Feature/ILockOnTarget/ILockOnTarget.h"

// パイロット機体クラス
class PilotMech;

/// <summary>
/// ロックオンシステム
/// </summary>
class PilotMechLockOnSystem {
public:
	PilotMechLockOnSystem(PilotMech* mech);
	~PilotMechLockOnSystem() = default;

	void Update();

	ILockOnTarget* GetTarget();

	bool GetIsLockOn() const;

	// 
	// 以下改修予定
	// 

	// 今はひとまずボスをロックオンするための処理
	void SetBoss(ILockOnTarget* bossMech);

private:
	// ロックオン対象の優先度を計算

	// プレイヤーの入力を加味

	// ロックオン対象を決定

	// ロックオン切り替え
	void SwitchLockOnFlag();

private:
	// ロックオン有効フラグ
	bool isLockOn_ = false;

	// ロックオン対象
	ILockOnTarget* target_ = nullptr;

	// 自機の参照ポインタ
	PilotMech* mech_ = nullptr;
};