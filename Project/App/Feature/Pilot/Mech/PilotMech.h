#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// ステート基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

/// <summary>
/// パイロット機体クラス
/// </summary>
class PilotMech:public BaseMech {
public:
	enum class PilotMechState {
		Idle,
		Move,
		Dodge,
		JustDodge,
		CounterAttack,
	};
public:
	PilotMech(const InitParam& param, const RefContext& ref);
	~PilotMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param);

	void ChangeState(PilotMech::PilotMechState nextState);

private:
	// ステート取得

	// ステート文字列変換
	const std::string StateToString(PilotMech::PilotMechState state);

	// デバッグウィンドウ描画
	void ShowDebugWindow()override;

private:
	// ステートテーブル
	std::unordered_map<PilotMech::PilotMechState, std::unique_ptr<IPilotMechState*>>states_;

	// 現在のステート
	std::pair<PilotMech::PilotMechState, IPilotMechState*> currentState_;
};
