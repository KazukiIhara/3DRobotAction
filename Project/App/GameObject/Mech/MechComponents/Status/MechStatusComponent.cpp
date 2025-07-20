#include "MechStatusComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"

MechStatusComponent::MechStatusComponent() {
	// HPを初期化
	hp_ = kMaxHP_;
}

void MechStatusComponent::Update(MechCore* mechCore) {
	// 衝突情報を取得
	std::vector<MechCollider::HitInfo> infos = mechCore->GetCollider()->GetHitInfo();
	// 衝突情報を順番に処理
	for (auto& info : infos) {
		if (info.isHit) {
			switch (info.type) {
				case AttackType::Bullet:
					hp_ -= info.damage;

					// 0以下にならないようにする
					hp_ = std::max(0, hp_);
					break;
			}

		}
	}
}


const int32_t& MechStatusComponent::GetHp() const {
	return hp_;
}

const int32_t& MechStatusComponent::GetMaxHp() const {
	return kMaxHP_;
}
