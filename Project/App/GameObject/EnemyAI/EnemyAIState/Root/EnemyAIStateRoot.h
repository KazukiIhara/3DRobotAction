#pragma once

#include "GameObject/EnemyAI/EnemyAIState/BaseEnemyAIState.h"
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 敵通常行動ステート
/// </summary>
class EnemyAIStateRoot :public BaseEnemyAIState {
public:
	~EnemyAIStateRoot() = default;

	void Enter(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Update(EnemyAI* enemyAI, MechCore* mechCore)override;
	void Exit(EnemyAI* enemyAI, MechCore* mechCore)override;
private:
	// 回避処理
	void Avoid(EnemyAI* enemyAI);
private:
	Vector2 targetMoveDir_ = { 0.0f,0.0f };
	Vector2 currentMoveDir_ = { 0.0f,0.0f };

	// 目標距離
	float targetRange_ = 30.0f;

	float jumpTimer_ = 0.0f;
	float boostTimer_ = 2.0f;

	float avoidCoolTimer_ = 0.5f;

	float shoulderWeaponTimer_ = 3.0f;

	float launcherWeaponTimer_ = 5.0f;
};