#pragma once

// C++
#include <memory>
#include <utility>
#include <vector>

#include "Feature/Damage/Collider/DamageCollider.h"

class BaseMech;

/// <summary>
/// ダメージ当たり判定システム（形状同士）
/// </summary>
class DamageCollisionSystem {
public:
	DamageCollisionSystem();
	~DamageCollisionSystem() = default;

	void Update();
	void Draw();

	void AddMech(BaseMech* mech);

	DamageCollider* AddCollider(std::unique_ptr<DamageCollider> collider);
	void Clear();

	const std::vector<std::pair<const DamageCollider*, const DamageCollider*>>& GetHitPairs() const;

public:
	static bool IsCollision(const DamageCollider::Param& a, const DamageCollider::Param& b);

	static bool IsCollisionSphereToSphere(const DamageCollider::Sphere& a, const DamageCollider::Sphere& b);
	static bool IsCollisionSphereToOBB(const DamageCollider::Sphere& s, const DamageCollider::OBB& b);
	static bool IsCollisionOBBToOBB(const DamageCollider::OBB& a, const DamageCollider::OBB& b);

	static bool IsCollisionCapsuleToSphere(const DamageCollider::Capsule& c, const DamageCollider::Sphere& s);
	static bool IsCollisionCapsuleToCapsule(const DamageCollider::Capsule& a, const DamageCollider::Capsule& b);
	static bool IsCollisionCapsuleToOBB(const DamageCollider::Capsule& c, const DamageCollider::OBB& b);

private:
	void RemoveDeadColliders();
	void UpdateColliders();
	void CheckCollision();

private:
	std::vector<BaseMech*> mechlist_;
	std::vector<std::unique_ptr<DamageCollider>> colliders_{};
	std::vector<std::pair<const DamageCollider*, const DamageCollider*>> hitPairs_{};
};
