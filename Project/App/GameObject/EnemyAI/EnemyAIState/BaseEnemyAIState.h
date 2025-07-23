#pragma once

class EnemyAI;
class MechCore;

/// <summary>
/// 敵AIのステート基底クラス
/// </summary>
class BaseEnemyAIState {
public:
	virtual ~BaseEnemyAIState() = default;
	virtual void Enter(EnemyAI* enemyAI, MechCore* mechCore) = 0;
	virtual void Update(EnemyAI* enemyAI, MechCore* mechCore) = 0;
	virtual void Exit(EnemyAI* enemyAI, MechCore* mechCore) = 0;
};