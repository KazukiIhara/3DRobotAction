#pragma once

class PilotMech;

/// <summary>
/// パイロット機体のステート既定クラス
/// </summary>
class IPilotMechState {
public:
	virtual ~IPilotMechState() = default;
	virtual void Enter(PilotMech* mech) = 0;
	virtual void Update(PilotMech* mech) = 0;
	virtual void Exit(PilotMech* mech) = 0;
};