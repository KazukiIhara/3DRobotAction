#pragma once

// C++
#include <memory>
#include <string>
#include <unordered_map>

#include "Feature/SceneStateController/CombatSceneController/CombatSceneControl.h"
#include "Feature/SceneStateController/CombatSceneController/States/ICombatSceneState.h"

/// <summary>
/// 戦闘シーンを管理、コントロールするクラス
/// </summary>
class CombatSceneController {
public:
	enum class State {
		None,
		Start,
		Battle,
		Dropped,
		End,
		Pause,
	};
public:
	CombatSceneController(CombatSceneControl::ContextRef ref);
	~CombatSceneController() = default;

	void Start(CombatSceneController::State state);
	void End();

	// 更新　戦闘シーンが終章したらFalseが返る
	bool Update();

	void Draw();

	void ChangeState(CombatSceneController::State state);

	std::string GetCurrentStateStr();

private:
	// ステート取得
	ICombatSceneState* GetState(CombatSceneController::State state);
	// ステートに対応した文字列を取得
	std::string GetStateStr(CombatSceneController::State state);

private:
	// ステートテーブル
	std::unordered_map<CombatSceneController::State, std::unique_ptr<ICombatSceneState>> states_;

	// ステートと文字を対応させたマップ
	std::unordered_map<CombatSceneController::State, std::string> statesStrMap_;

	// 現在のステート
	std::pair<CombatSceneController::State, ICombatSceneState*> currentState_{};

	// シーンへの終了通知用フラグ
	bool isEnd_ = false;

	// 参照ポインタ構造体
	CombatSceneControl::StateContextRef ref_;
};