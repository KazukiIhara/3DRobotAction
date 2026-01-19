#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// ステート基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

// システムクラス
#include "Feature/Pilot/Mech/System/Move/PilotMechMoveSystem.h"
#include "Feature/Pilot/Mech/System/ModelDir/PilotMechModelDirSystem.h"

class GameInputSystem;

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
	PilotMech(const InitParam& param, const RefContext& ref, GameInputSystem* InputSys);
	~PilotMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param);

	void ChangeState(PilotMech::State nextState);

	// システムを取得
	PilotMechMoveSystem* GetMoveSystem();
	GameInputSystem* GetInputSys();

private:
	// ステート取得
	IPilotMechState* GetState(PilotMech::State state);

	// ステート文字列変換
	const std::string StateToString(PilotMech::State state);

	// デバッグウィンドウ描画
	void ShowDebugWindow()override;

private:
	// ステートテーブル
	std::unordered_map<PilotMech::State, std::unique_ptr<IPilotMechState>>states_;

	// 現在のステート
	std::pair<PilotMech::State, IPilotMechState*> currentState_;

	// 移動システム
	std::unique_ptr<PilotMechMoveSystem> moveSystem_;
	// モデルの向き管理システム
	std::unique_ptr<PilotMechModelDirSystem> modelDirSystem_;

	// 入力システムの参照ポインタ
	GameInputSystem* inputSys_ = nullptr;
};
