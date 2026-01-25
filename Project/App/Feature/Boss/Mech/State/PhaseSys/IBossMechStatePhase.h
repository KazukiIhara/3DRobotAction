#pragma once

class BossMech;

class IBossMechStatePhase {
public:
	virtual ~IBossMechStatePhase() = default;

	virtual void Enter(BossMech* mech) = 0;
	virtual void Update(BossMech* mech) = 0;
	virtual void Exit(BossMech* mech) = 0;

	virtual bool EndRequest() = 0;
};
