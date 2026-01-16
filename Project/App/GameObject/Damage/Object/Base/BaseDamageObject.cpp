#include "BaseDamageObject.h"

#include "MAGI.h"

#include "GameObject/Damage/Collider/DamageCollider.h"

using namespace Magi;

BaseDamageObject::BaseDamageObject(const Vector3& worldPos) {
	transform_ = MAGISYSTEM::AddTransform3D(worldPos);
}

void BaseDamageObject::SetCollisonManager(DamageCollisonManager* collisionManager) {
	collisionManager_ = collisionManager;
}

BaseDamageCollider* BaseDamageObject::GetDamageCollider() {
	return damageCollider_;
}
