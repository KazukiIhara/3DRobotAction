#pragma once

// C++
#include <memory>
#include <unordered_map>

// 前方宣言
class BossMech;
class MechCore;
class BaseBossAIState;
class AttackCollisionManager;

/// <summary>
/// ボスAIクラス
/// </summary>
class BossAI {
public:
	enum class State {
		Idle
	};

	BossAI(BossMech* mech, MechCore* playerMech, AttackCollisionManager* collisionManager);
	~BossAI() = default;

	void Update();

	void ChangeState(BossAI::State nextState);


private:
	// 機体の参照ポインタ
	BossMech* mech_ = nullptr;
	// プレイヤーの参照ポインタ
	MechCore* player_ = nullptr;
	// 攻撃判定マネージャのポインタ
	AttackCollisionManager* attackCollisionManager_ = nullptr;

};