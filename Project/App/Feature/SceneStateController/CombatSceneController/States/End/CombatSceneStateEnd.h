#pragma once

#include "Feature/SceneStateController/CombatSceneController/States/ICombatSceneState.h"

class CombatSceneStateEnd: public ICombatSceneState {
public:
	~CombatSceneStateEnd()override = default;
	void Enter(CombatSceneControl::StateContextRef ref)override;
	void Update(CombatSceneControl::StateContextRef ref)override;
	void Draw(CombatSceneControl::StateContextRef ref)override;
	void Exit(CombatSceneControl::StateContextRef ref)override;
private:
	float timer_ = 0.0f;
};
