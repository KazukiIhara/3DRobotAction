#include "MechStatusComponent.h"

MechStatusComponent::MechStatusComponent() {
	// HPを初期化
	hp_ = kMaxHP_;
}

void MechStatusComponent::AddDamage(int32_t damage) {
	hp_ -= damage;
}

const int32_t& MechStatusComponent::GetHp() const {
	return hp_;
}
