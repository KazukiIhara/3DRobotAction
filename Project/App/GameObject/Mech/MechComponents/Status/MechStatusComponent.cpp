#define NOMINMAX

#include "MechStatusComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"
#include "MAGI.h"

MechStatusComponent::MechStatusComponent() {
	// HPを初期化
	hp_ = kMaxHP_;
	// HPの割合を計算
	hpRaito_ = float(hp_) / float(kMaxHP_);

	// ENを初期化
	en_ = kMaxEN_;
	// ENの割合を計算
	enRaito_ = float(en_) / float(kMaxEN_);

	// オーバーヒートの情報を初期化
	overHeatTimer_ = 0.0f;
	isOverHeat_ = false;
}

void MechStatusComponent::Update(MechCore* mechCore) {
	// 衝突情報を取得
	std::vector<MechCollider::HitInfo> infos = mechCore->GetCollider()->GetHitInfo();
	// 衝突情報を順番に処理
	for (auto& info : infos) {
		if (info.isHit) {
			switch (info.type) {
			case AttackType::Bullet:

				// ダメージを受ける
				GetDamage(info.damage, mechCore);
				// 衝突時エフェクト発生
				mechCore->GetBulletHitEffect()->Emit(info.attackPos);

				break;
			case AttackType::Missile:
				GetDamage(info.damage, mechCore);
				break;
			}
		}
	}

	// オーバーヒートしているかどうかの処理
	if (en_ == 0) {
		isOverHeat_ = true;
	}

	// ENの回復処理
	if (en_ < kMaxEN_) {
		en_ += static_cast<int32_t>(kEnRecoveryPerSec_ * MAGISYSTEM::GetDeltaTime());
	} else {
		en_ = kMaxEN_;
	}

	// HPの割合を計算
	hpRaito_ = float(hp_) / float(kMaxHP_);

	// ENの割合を計算
	enRaito_ = float(en_) / float(kMaxEN_);

	// FCS係数の更新
	UpdateFcsAvoidFactor(mechCore);

}


int32_t MechStatusComponent::GetHp() const {
	return hp_;
}

int32_t MechStatusComponent::GetMaxHp() const {
	return kMaxHP_;
}

float MechStatusComponent::GetHPRaito() const {
	return hpRaito_;
}

int32_t MechStatusComponent::GetEn() const {
	return en_;
}

int32_t MechStatusComponent::GetMaxEn() const {
	return kMaxEN_;
}

float MechStatusComponent::GetEnRaito() const {
	return enRaito_;
}

const bool& MechStatusComponent::GetIsOverheat() const {
	return isOverHeat_;
}

void MechStatusComponent::UseQuickBoostEnergy() {
	UseEnergy(kQuickBoostUseEn_);
}

float MechStatusComponent::GetFcsAvoidFactor() const {
	return fcsAvoidFactor_;
}

void MechStatusComponent::UseEnergy(const int32_t& enValue) {
	en_ -= enValue;
	// 0より小さくならないようにする
	en_ = std::max(en_, 0);
}

void MechStatusComponent::GetDamage(const int32_t& damage, MechCore* mechcore) {
	hp_ -= damage;
	// 0未満にならないようにする
	hp_ = std::max(0, hp_);

	//
	// コントローラを振動させる(攻撃の種類が増えたらダメージに応じて振動を変える)
	//
	if (mechcore->GetFriendlyTag() == FriendlyTag::PlayerSide) {
		MAGISYSTEM::StartPadVibration(0, 0.2f, 1.0f, 1.0f);
	}
}

void MechStatusComponent::UpdateFcsAvoidFactor(MechCore* mechCore) {
	// 対象を取得
	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		// 相手が回避状態
		if (target->GetCurrentState() == MechCoreState::QuickBoost) {
			fcsAvoidFactor_ = 0.02f;
		} else {
			// FCS復帰
			fcsAvoidFactor_ += (1.0f / fcsRecoverTime_) * MAGISYSTEM::GetDeltaTime();
			// 1.0fを超えないようにする
			fcsAvoidFactor_ = std::min(fcsAvoidFactor_, 1.0f);
		}
	}
}
