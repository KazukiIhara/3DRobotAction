#pragma once

#include "Feature/SceneStateController/CombatSceneController/States/ICombatSceneState.h"

class CombatSceneStatePause: public ICombatSceneState {
public:
	~CombatSceneStatePause()override = default;
	void Enter(CombatSceneControl::StateContextRef ref)override;
	void Update(CombatSceneControl::StateContextRef ref)override;
	void Draw(CombatSceneControl::StateContextRef ref)override;
	void Exit(CombatSceneControl::StateContextRef ref)override;
private:

};