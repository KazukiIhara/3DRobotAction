#pragma once

#include "Feature/SceneStateController/CombatSceneController/CombatSceneControl.h"

/// <summary>
/// 戦闘シーン内のステートインターフェース
/// </summary>
class ICombatSceneState {
public:
	virtual ~ICombatSceneState() = default;
	virtual void Enter(CombatSceneControl::StateContextRef ref) = 0;
	virtual void Update(CombatSceneControl::StateContextRef ref) = 0;
	virtual void Draw(CombatSceneControl::StateContextRef ref) = 0;
	virtual void Exit(CombatSceneControl::StateContextRef ref) = 0;
};