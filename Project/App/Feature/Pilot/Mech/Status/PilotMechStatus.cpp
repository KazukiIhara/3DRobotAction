#include "PilotMechStatus.h"

#include "Feature/Pilot/Mech/PilotMech.h"

PilotMechStatus::PilotMechStatus(PilotMech* mech) {
	mech_ = mech;

	// パラメータセット

}

void PilotMechStatus::Update() {
	// 衝突座標をリセット
	hitPos_ = { 0.0f,0.0f,0.0f };

	// 被弾を処理する
	ReactHitInfo();
}

Vector3 PilotMechStatus::GetHitPos() const {
	return hitPos_;
}

void PilotMechStatus::ReactHitInfo() {
	// 機体の現在ステートを取得
	auto pMechState = mech_->GetCurrentState();
	// ジャスト回避中は処理しない
	if (pMechState == PilotMech::State::JustDodge) {
		return;
	}
	// ノックバックステートなら処理しない
	if (pMechState == PilotMech::State::KnockBack) {
		return;
	}

	// ヒットインフォを取得
	auto& hitInfoList = mech_->GetCollider()->GetHitInfoList();

	// 空なら早期リターン
	if (hitInfoList.empty()) {
		return;
	}

	// 全被弾情報の処理
	for (auto& info : hitInfoList) {
		// ダメージ処理

		// 攻撃の威力ごとの処理
		Damage::Power power = info.power;
		switch (power) {
		case Damage::Power::Large:
			// 衝突座標を保存
			hitPos_ = info.hitPos;
			// ノックバックステートに遷移
			mech_->ChangeState(PilotMech::State::KnockBack);
			break;
		case Damage::Power::Mid:
			// ヒットリアクションステートに遷移
			mech_->ChangeState(PilotMech::State::HitReact);
			break;
		case Damage::Power::Small:

			break;
		}
	}


}
