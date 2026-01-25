#pragma once

class PilotMech;

/// <summary>
/// パイロット機体縦移動システム
/// </summary>
class PilotMechVerticalMoveSystem {
public:
	explicit PilotMechVerticalMoveSystem(PilotMech* mech);
	~PilotMechVerticalMoveSystem() = default;

	void PreUpdate();
	void PostUpdate();

private:
	PilotMech* mech_ = nullptr;

	float vy_ = 0.0f;
	bool isThrusting_ = false;
};
