#pragma once

// C++
#include <memory>
#include <vector>

#include "Feature/Damage/Collider/DamageCollider.h"

class BaseMech;

class DamageCollisionSystem {
public:
	struct HitPair {
		BaseMech* mech = nullptr;
		DamageCollider* damage = nullptr;
	};

public:
	DamageCollisionSystem();

	void Update();
	void Draw();

	void AddMech(BaseMech* mech);
	DamageCollider* AddCollider(std::unique_ptr<DamageCollider> collider);

	void Clear();

	void SwitchIsDebugDraw();

	bool& GetIsDebugDraw();

private:
	void RemoveDeadColliders();
	void UpdateDamageColliders();
	void CheckCollision();

	bool IsCollision(const DamageCollider::Param& a, const DamageCollider::Param& b);

	bool IsCollisionSphereToSphere(const DamageCollider::Sphere& a, const DamageCollider::Sphere& b);
	bool IsCollisionSphereToOBB(const DamageCollider::Sphere& s, const DamageCollider::OBB& b);
	bool IsCollisionOBBToOBB(const DamageCollider::OBB& a, const DamageCollider::OBB& b);
	bool IsCollisionCapsuleToSphere(const DamageCollider::Capsule& c, const DamageCollider::Sphere& s);
	bool IsCollisionCapsuleToCapsule(const DamageCollider::Capsule& a, const DamageCollider::Capsule& b);
	bool IsCollisionCapsuleToOBB(const DamageCollider::Capsule& c, const DamageCollider::OBB& b);

private:
	std::vector<BaseMech*> mechlist_;
	std::vector<std::unique_ptr<DamageCollider>> colliders_;

	bool isDebugDraw_ = false;
};
