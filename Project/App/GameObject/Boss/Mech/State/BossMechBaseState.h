#pragma once

// ボス機体
class BossMech;

// ボス機体基底クラス
class BossMechBaseState {
public:
	virtual ~BossMechBaseState() = default;
	virtual void Enter(BossMech* mechCore) = 0;
	virtual void Update(BossMech* mechCore) = 0;
	virtual void Exit(BossMech* mechCore) = 0;

};