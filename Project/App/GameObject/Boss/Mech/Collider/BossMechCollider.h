#pragma once

// C++
#include <array>
#include "Math/Types/AllMathTypes.h"

#include "MechAnimation/MechAnimation.h"

// 前方宣言
class BossMech;

/// <summary>
/// ボス機体用コライダー
/// </summary>
class BossMechCollider {
public:
	struct HitInfo {
		bool isHit = false;
	};
	struct Capsule {
		Vector3 p0;
		Vector3 p1;
		float radius;
	};
public:
	BossMechCollider(BossMech* mech);
	~BossMechCollider() = default;

	void Update();

	void Draw();

	void SetCapsule(size_t idx, MechAnimation::TransType a, MechAnimation::TransType b);
private:
	std::array<Capsule, 9> colliders_;
	BossMech* mech_;
};