#pragma once

// C++
#include <cstdint>

#include "Math/Types/AllMathTypes.h"

// パイロット機体
class PilotMech;

/// <summary>
/// パイロット機体のステータス
/// </summary>
class PilotMechStatus {
public:
	struct Param {

	};
public:
	PilotMechStatus(PilotMech* mech);
	~PilotMechStatus() = default;

	void Update();

	Vector3 GetHitPos()const;

private:
	void ReactHitInfo();
private:
	Vector3 hitPos_;
	Param param_{};
	PilotMech* mech_ = nullptr;
};