#pragma once

#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"

class EnemyAIStateRoot:public BaseEnemyAIState {
public:
	~EnemyAIStateRoot() = default;

	void Enter(EnemyAI* enemyAI)override;
	void Update(EnemyAI* enemyAI)override;
	void Exit(EnemyAI* enemyAI)override;

private:
	float jumpTimer_ = 0.0f;
	const float jumpTime_ = 1.0f;
};