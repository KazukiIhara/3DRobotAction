#pragma once

#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"

class EnemyAIStateRoot:public BaseEnemyAIState {
public:
	~EnemyAIStateRoot() = default;

	void Enter(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Update(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Exit(EnemyAI* enemyAI, MechCore* mechCore)override;

private:
	float jumpTimer_ = 0.0f;
	const float jumpTime_ = 3.0f;
};