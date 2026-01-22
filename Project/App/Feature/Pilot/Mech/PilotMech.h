#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// ステート基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

// ジャスト回避コライダー
#include "Feature/Pilot/Mech/JustDodgeCollider/PilotMechJustDodgeCollider.h"

// 前方宣言
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
		JustDodgeAttack,
	};
public:
	PilotMech(const InitParam& param, const RefContext& ref, GameInputSystem* InputSys);
	~PilotMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param);

	void DebugDraw()override;

	void ChangeState(PilotMech::State nextState);

	PilotMech::State GetCurrentState()const;

	PilotMechJustDodgeCollider* GetJustDodgeCollider();

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

	// ジャスト回避コライダー
	std::unique_ptr<PilotMechJustDodgeCollider> justDodgeCollider_;

	// 入力システムの参照ポインタ
	GameInputSystem* inputSys_ = nullptr;
};
