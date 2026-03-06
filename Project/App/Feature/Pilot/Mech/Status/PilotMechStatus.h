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
		int32_t hp;
		bool dropped;
	};
public:
	PilotMechStatus(PilotMech* mech);
	~PilotMechStatus() = default;

	void Update();

	Vector3 GetHitPos()const;

	int32_t GetHP()const;
	int32_t GetMaxHP()const;

	bool GetIsDropped()const;

private:
	void ReactHitInfo();
	void JudgeDropped();
private:
	Vector3 hitPos_;
	Param param_{};
	float droppedHeight_ = 0.0f;
	int32_t maxHp_;
	PilotMech* mech_ = nullptr;
};