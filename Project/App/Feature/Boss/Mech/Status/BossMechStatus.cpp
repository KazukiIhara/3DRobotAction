#define NOMINMAX

#include "BossMechStatus.h"

#include "Feature/Boss/Mech/BossMech.h"

BossMechStatus::BossMechStatus() {

}

void BossMechStatus::Damage(float damage) {
	hp_ -= damage;
	hp_ = std::max(0.0f, hp_);
}

float BossMechStatus::GetHP() const {
	return hp_;
}
