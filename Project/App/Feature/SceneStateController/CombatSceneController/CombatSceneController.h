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
		Start,
		Battle,
		End,
		Pause,
	};
public:
	CombatSceneController(CombatSceneControl::ContextRef ref);
	~CombatSceneController() = default;

	void Start();
	void Update();
	void Draw();
	
	void ChangeState(CombatSceneController::State state);

	// ステート取得
	ICombatSceneState* GetState(CombatSceneController::State state);

private:
	// ステートテーブル
	std::unordered_map<CombatSceneController::State, std::unique_ptr<ICombatSceneState>> states_;
	// 現在のステート
	std::pair<CombatSceneController::State, ICombatSceneState*> currentState_;

	// 参照ポインタ構造体
	CombatSceneControl::ContextRef ref_;
};