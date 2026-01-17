#include "BaseDamageObject.h"

#include "MAGI.h"

#include "GameObject/Damage/Collider/DamageCollider.h"

using namespace Magi;

BaseDamageObject::BaseDamageObject(const Vector3& worldPos) {
	transform_ = MAGISYSTEM::AddTransform3D(worldPos);
}

void BaseDamageObject::SetCollisionSystem(DamageCollisionSystem* collisionSystem) {
	collisionSystem_ = collisionSystem;
}

void BaseDamageObject::Finalize() {
	isAlive_ = false;
	if (damageCollider_) {
		damageCollider_->SetIsAlive(false);
	}

	transform_->SetIsAlive(false);
}

DamageCollider* BaseDamageObject::GetDamageCollider() {
	return damageCollider_;
}

bool BaseDamageObject::GetIsAlive()const {
	return isAlive_;
}
