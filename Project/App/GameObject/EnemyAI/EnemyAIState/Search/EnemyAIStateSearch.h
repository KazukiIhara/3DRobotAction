#pragma once

#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"
#include "Math/Types/AllMathTypes.h"

class EnemyAIStateSearch :public BaseEnemyAIState {
public:
	~EnemyAIStateSearch() = default;

	void Enter(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Update(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Exit(EnemyAI* enemyAI, MechCore* mechCore)override;

};