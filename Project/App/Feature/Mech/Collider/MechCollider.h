#pragma once

// C++
#include <array>
#include "Math/Types/AllMathTypes.h"

#include "MechAnimation/MechAnimation.h"

// 前方宣言
class BaseMech;

/// <summary>
/// 機体用コライダー
/// </summary>
class MechCollider {
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
	MechCollider(BaseMech* mech);
	~MechCollider() = default;

	void Update();

	void Draw();

	const std::array<MechCollider::Capsule, 9>& GetList()const;

private:
	void SetCapsule(size_t idx, MechAnimation::TransType a, MechAnimation::TransType b);
private:
	std::array<Capsule, 9> colliders_;
	BaseMech* mech_;
};