#include "DamageCollider.h"

DamageCollider::DamageCollider(Param param) {
	param_ = std::move(param);

}

const DamageCollider::Param& DamageCollider::GetParam() const {
	return param_;
}