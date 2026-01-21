#include "BaseGameEffect.h"

BaseGameEffect::BaseGameEffect(const Vector3& emitPos) {
	worldPos_ = emitPos;
}

bool BaseGameEffect::IsAlive() const {
	return isAlive_;
}

void BaseGameEffect::SetWorldPos(const Vector3& worldPos) {
	worldPos_ = worldPos;
}