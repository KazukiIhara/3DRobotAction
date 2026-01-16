#include "BaseDamageObject.h"

#include "MAGI.h"

#include "GameObject/Damage/Collider/IDamageCollider.h"

using namespace Magi;

BaseDamageObject::BaseDamageObject(const Vector3& worldPos, BaseDamageCollider* damageCollider) {
	transform_ = MAGISYSTEM::AddTransform3D(worldPos);
	damageCollider_ = damageCollider;
}

BaseDamageCollider* BaseDamageObject::GetDamageCollider() {
	return damageCollider_;
}
