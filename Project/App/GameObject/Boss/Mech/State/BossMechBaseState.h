#pragma once


// ボス機体
class BossMech;

/// <summary>
///	ボス機体ステート基底クラス
/// </summary>
class BossMechBaseState {
public:
	virtual ~BossMechBaseState() = default;
	virtual void Enter(BossMech* mechCore) = 0;
	virtual void Update(BossMech* mechCore) = 0;
	virtual void Exit(BossMech* mechCore) = 0;
};