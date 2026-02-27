#pragma once

#include "Math/Types/AllMathTypes.h"

class PilotMech;

// 実装メモ

/*
	ステートとは別に機体を動かしたりするクラス
	右手武器の攻撃、移動時の機体の傾きなど
*/

class PilotMech;

/// <summary>
/// パイロット機体アクションシステム
/// </summary>
class PilotMechCombatActionSystem {
public:
	enum class RightWeaponAttackState {
		Idle,
		SetUp,
		Attack
	};
public:
	PilotMechCombatActionSystem(PilotMech* mech);
	~PilotMechCombatActionSystem() = default;

	void Update();

private:
	// 移動方向に応じて機体を傾ける処理
	void MechSlopeUpdate();

	// 右手武器攻撃可能フラグ
	void SetEnableRightWeapon(bool e);

	// 右手武器攻撃ステートを更新
	void RightWeaponAttackStateUpdate();

	void UpdateIdle();
	void UpdateSetUp();
	void UpdateAttack();

private:
	// 機体の参照ポインタ
	PilotMech* mech_ = nullptr;

	// 右手武器攻撃可能フラグ
	bool enableRightWeapon_ = true;

	// 右手武器攻撃ステート
	RightWeaponAttackState rightWeaponAttackState_ = RightWeaponAttackState::Idle;

	// 右手目標角度
	Quaternion targetArmQ_;

};