#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// ステート基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

// システムクラス
#include "Feature/Pilot/Mech/System/PilotMechMoveSystem.h"

/// <summary>
/// パイロット機体クラス
/// </summary>
class PilotMech:public BaseMech {
public:
	enum class State {
		Idle,
		Move,
		Dodge,
		JustDodge,
	};
public:
	PilotMech(const InitParam& param, const RefContext& ref);
	~PilotMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param);

	void ChangeState(PilotMech::State nextState);

private:
	// ステート取得
	IPilotMechState* GetState(PilotMech::State state);

	// ステート文字列変換
	const std::string StateToString(PilotMech::State state);

	// システムを取得
	PilotMechMoveSystem* GetMoveSystem();

	// デバッグウィンドウ描画
	void ShowDebugWindow()override;

private:
	// ステートテーブル
	std::unordered_map<PilotMech::State, std::unique_ptr<IPilotMechState>>states_;

	// 現在のステート
	std::pair<PilotMech::State, IPilotMechState*> currentState_;

	// 移動システム
	std::unique_ptr<PilotMechMoveSystem> moveSystem_;

};
