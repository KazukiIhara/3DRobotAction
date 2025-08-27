#pragma once

#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"
#include "Math/Types/AllMathTypes.h"

class EnemyAIStateRoot :public BaseEnemyAIState {
public:
	~EnemyAIStateRoot() = default;

	void Enter(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Update(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Exit(EnemyAI* enemyAI, MechCore* mechCore)override;

private:
	Vector2 currentMoveDir_ = { 1.0f,0.2f };
	float targetRange_ = 3.0f;

	float jumpTimer_ = 0.0f;
	const float jumpTime_ = 3.0f;
};