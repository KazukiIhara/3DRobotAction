#pragma once

#include "Feature/SceneStateController/CombatSceneController/States/ICombatSceneState.h"

class CombatSceneStateStart: public ICombatSceneState {
public:
	~CombatSceneStateStart()override = default;
	void Enter(CombatSceneControl::ContextRef ref)override;
	void Update(CombatSceneControl::ContextRef ref)override;
	void Draw(CombatSceneControl::ContextRef ref)override;
	void Exit(CombatSceneControl::ContextRef ref)override;
private:

};