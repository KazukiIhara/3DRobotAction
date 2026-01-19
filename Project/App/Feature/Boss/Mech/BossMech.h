#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// 武器クラス
#include "Feature/Boss/Mech/Weapon/LaserGun/BossMechWeaponLaserGun.h"

// ステート基底クラス
#include "Feature/Boss/Mech/State/BossMechBaseState.h"

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech:public BaseMech {
public:
	// ステート
	enum class State {
		Idle,
		LaserShot
	};
public:
	BossMech(const InitParam& param, const RefContext& ref);
	~BossMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param)override;

	void ChangeState(BossMech::State nextState);

private:
	// 対応するステートを取得
	BossMechBaseState* GetState(BossMech::State state);
	// ステートを文字列に変換
	const std::string StateToString(BossMech::State state);

	// デバッグウィンドウ描画
	void ShowDebugWindow()override;

private:
	// ステートテーブル
	std::unordered_map<BossMech::State, std::unique_ptr<BossMechBaseState>> states_;

	// 現在のステート
	std::pair<BossMech::State, BossMechBaseState*> currentState_;

};