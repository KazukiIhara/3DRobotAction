#pragma once


// ボス機体
class BossMech;

/// <summary>
///	ボス機体ステート基底クラス
/// </summary>
class BossMechBaseState {
public:
	virtual ~BossMechBaseState() = default;
	virtual void Enter(BossMech* mech) = 0;
	virtual void Update(BossMech* mech) = 0;
	virtual void Exit(BossMech* mech) = 0;
};