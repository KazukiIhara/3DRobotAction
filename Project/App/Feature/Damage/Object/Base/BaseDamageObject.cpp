#include "BaseDamageObject.h"

#include "MAGI.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

using namespace Magi;

BaseDamageObject::BaseDamageObject(const Vector3& worldPos, DamageCollisionSystem* damageCollisionSystem) {
	transform_ = MAGISYSTEM::AddTransform3D(worldPos);
	collisionSystem_ = damageCollisionSystem;
}

void BaseDamageObject::Finalize() {
	isAlive_ = false;
	if (collider_) {
		collider_->SetIsAlive(false);
	}

	transform_->SetIsAlive(false);
}

void BaseDamageObject::AddDamageCollider(DamageCollider::Param param, const DamageCollider::GameParam& gParam) {
	std::unique_ptr<DamageCollider> collider =
		std::make_unique<DamageCollider>(param, gParam);
	collider_ = GetCollisionSystem()->AddCollider(std::move(collider));
}

DamageCollider* BaseDamageObject::GetDamageCollider() {
	return collider_;
}

DamageCollisionSystem* BaseDamageObject::GetCollisionSystem() {
	return collisionSystem_;
}

bool BaseDamageObject::GetIsAlive()const {
	return isAlive_;
}
