#pragma once

#include "Feature/SceneStateController/CombatSceneController/CombatSceneControl.h"

/// <summary>
/// 戦闘シーン内のステートインターフェース
/// </summary>
class ICombatSceneState {
public:
	virtual ~ICombatSceneState() = default;
	virtual void Enter(CombatSceneControl::ContextRef ref) = 0;
	virtual void Update(CombatSceneControl::ContextRef ref) = 0;
	virtual void Draw(CombatSceneControl::ContextRef ref) = 0;
	virtual void Exit(CombatSceneControl::ContextRef ref) = 0;
};