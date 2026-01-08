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
			// ダメージを受ける
			GetDamage(info.damage, mechCore);

			switch (info.type) {
			case AttackType::Bullet:
				// 衝突時エフェクト発生
				mechCore->GetBulletHitEffect()->Emit(info.attackPos);

				break;
			case AttackType::Rocket:

				break;
			case AttackType::Missile:

				break;
			}
		}
	}

	// HPの割合を計算
	hpRaito_ = float(hp_) / float(kMaxHP_);

	// EN更新処理
	ENUpdate();

	// FCS係数の更新
	UpdateFcsAvoidFactor(mechCore);

	// 硬直の更新
	UpdateRecoveryTime();
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

void MechStatusComponent::UseUpBoostEnergy() {
	UseEnergy(static_cast<int>(static_cast<float>(kUpBoostUseEnPerSec_) * MAGISYSTEM::GetDeltaTime()));
}

void MechStatusComponent::UseQuickBoostEnergy() {
	UseEnergy(kQuickBoostUseEn_);
}

float MechStatusComponent::GetFcsAvoidFactor() const {
	return fcsAvoidFactor_;
}

float MechStatusComponent::GetRecoveryTime() const {
	return recoveryTime_;
}

void MechStatusComponent::SetRecoveryTime(float t) {
	recoveryTime_ = t;
}

void MechStatusComponent::SetDemoMode(bool inv) {
	demoMode_ = inv;
}

void MechStatusComponent::UseEnergy(const int32_t& enValue) {
	// ENを消費
	en_ -= enValue;
	// 0より小さくならないようにする
	en_ = std::max(en_, 0);

	// クールタイムをセット
	enRecoveryCoolTimer_ = kEnRecoveryCoolTime_;
}

void MechStatusComponent::GetDamage(const int32_t& damage, MechCore* mechCore) {

	// HPを減らす
	hp_ -= damage;
	// 0未満にならないようにする
	hp_ = std::max(0, hp_);

	// DEMOモードなら0になった後ループ
	if (demoMode_) {
		if (hp_ == 0) {
			hp_ = kMaxHP_;
		}
		return;
	}

	if (mechCore->GetFriendlyTag() == FriendlyTag::PlayerSide) {
		//
		// コントローラを振動させる(攻撃の種類が増えたらダメージに応じて振動を変える)
		//
		MAGISYSTEM::StartPadVibration(0, 0.2f, 1.0f, 1.0f);

		//
		// カメラ揺らす
		//
		MAGISYSTEM::ShakeCurrentCamera3D(0.3f, Vector3(0.1f, 0.2f, 0.0f));
	}

}

void MechStatusComponent::UpdateFcsAvoidFactor(MechCore* mechCore) {
	// 対象を取得
	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		// 相手が回避状態
		if (target->GetCurrentState() == MechCoreState::QuickBoost) {
			fcsAvoidFactor_ = minFcsFactor_;
		} else {
			// FCS復帰
			fcsAvoidFactor_ += (1.0f / fcsRecoverTime_) * MAGISYSTEM::GetDeltaTime();
			// 1.0fを超えないようにする
			fcsAvoidFactor_ = std::min(fcsAvoidFactor_, 1.0f);
		}
	}
}

void MechStatusComponent::UpdateRecoveryTime() {
	// 硬直時間を更新
	recoveryTime_ -= MAGISYSTEM::GetDeltaTime();
	// 0より小さくならないようにする
	recoveryTime_ = std::max(recoveryTime_, 0.0f);

}

void MechStatusComponent::ENUpdate() {
	// EN回復クールタイマーを更新
	enRecoveryCoolTimer_ -= MAGISYSTEM::GetDeltaTime();
	// 0を下回らないようにする
	enRecoveryCoolTimer_ = std::max(enRecoveryCoolTimer_, 0.0f);

	// EN回復処理
	if (enRecoveryCoolTimer_ == 0.0f) {
		en_ += static_cast<int32_t>(kEnRecoveryPerSec_ * MAGISYSTEM::GetDeltaTime());
	}
	// 最大値を超えないようにする
	en_ = std::min(en_, kMaxEN_);

	// ENの割合を計算
	if (en_ != 0.0f) {
		enRaito_ = float(en_) / float(kMaxEN_);
	} else {
		enRaito_ = 0.0f;
	}

	// オーバーヒート処理
	if (enRaito_ == 0.0f) {
		isOverHeat_ = true;
	} else if (enRaito_ >= 0.5f) {
		// 半分以上になったらオーバーヒート解除
		isOverHeat_ = false;
	}

}
