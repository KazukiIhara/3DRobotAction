#pragma once

class PilotMech;

/// <summary>
/// 
/// </summary>
class IPilotMechState {
public:
	virtual ~IPilotMechState() = default;
	virtual void Enter(PilotMech* mech) = 0;
	virtual void Update(PilotMech* mech) = 0;
	virtual void Exit(PilotMech* mech) = 0;
};