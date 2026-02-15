#define NOMINMAX

#include "BossMechStatus.h"

#include "Feature/Boss/Mech/BossMech.h"

#include "MAGI.h"

using namespace Magi;

BossMechStatus::BossMechStatus(BossMech* mech) {
	mech_ = mech;

	// パラメータセット
	param_.hp = MAGISYSTEM::GetParameterValue<int32_t>({ "MechInitParam","Boss","HP" });
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
	return param_.hp;
}

void BossMechStatus::Damage(int32_t damage) {
	param_.hp -= damage;
	param_.hp = std::max(0, param_.hp);
}

