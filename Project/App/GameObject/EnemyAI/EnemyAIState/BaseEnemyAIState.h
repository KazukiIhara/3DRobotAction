#pragma once

class EnemyAI;

/// <summary>
/// 敵AIのステート基底クラス
/// </summary>
class BaseEnemyAIState {
public:
	virtual ~BaseEnemyAIState() = default;
	virtual void Enter(EnemyAI* enemyAI) = 0;
	virtual void Update(EnemyAI* enemyAI) = 0;
	virtual void Exit(EnemyAI* enemyAI) = 0;
};