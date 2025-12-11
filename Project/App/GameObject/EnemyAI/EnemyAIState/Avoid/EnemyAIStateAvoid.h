#pragma once

#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"
#include "Math/Types/AllMathTypes.h"


namespace magi {
/// <summary>
/// 敵回避ステート
/// </summary>
class EnemyAIStateAvoid :public BaseEnemyAIState {
public:
	~EnemyAIStateAvoid() = default;

	void Enter(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Update(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Exit(EnemyAI* enemyAI, MechCore* mechCore)override;

};
}
