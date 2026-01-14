#pragma once

// C++
#include <memory>
#include <unordered_map>

// 前方宣言
class BossMech;
class MechCore;
class AttackCollisionManager;

// 実装メモ

/*
	ボス機体のステートを状況に応じて変更させるクラス
	最初は一定時間ごとにステートを乱数で変える実装
	余裕があればUtilityAIなどを検討する
*/

/// <summary>
/// ボスAIクラス
/// </summary>
class BossAI {
public:
	BossAI(BossMech* mech, MechCore* playerMech, AttackCollisionManager* collisionManager);
	~BossAI() = default;

	void Update();

private:
	// 機体の参照ポインタ
	BossMech* mech_ = nullptr;
	// プレイヤーの参照ポインタ
	MechCore* player_ = nullptr;
	// 攻撃判定マネージャのポインタ
	AttackCollisionManager* attackCollisionManager_ = nullptr;

};