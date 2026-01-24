#define NOMINMAX

#include "BossMechStatus.h"

#include "Feature/Boss/Mech/BossMech.h"


BossMechStatus::BossMechStatus(const BossMechStatus::InitParam& initPram,BossMech* mech) {
	mech_ = mech;

	hp_ = initPram.hp;

}

void BossMechStatus::Update() {
	// 衝突情報を取得
	auto& hitList = mech_->GetCollider()->GetHitInfoList();

	// リストに入っているダメージを与える
	for (auto& hitInfo : hitList) {
		Damage(hitInfo.damage);
	}

}

int32_t BossMechStatus::GetHP() const {
	return hp_;
}

void BossMechStatus::Damage(int32_t damage) {
	hp_ -= damage;
	hp_ = std::max(0, hp_);
}

