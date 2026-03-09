#include "PilotMechStatus.h"

#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

PilotMechStatus::PilotMechStatus(PilotMech* mech) {
	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "MechInitParam","Pilot","HP" }, ParamType::Int32);
	MAGISYSTEM::AddParameterData({ "MechInitParam","Pilot","DroppedHeight" }, ParamType::Float);

	mech_ = mech;

	// パラメータセット
	param_.hp = MAGISYSTEM::GetParameterValue<int32_t>({ "MechInitParam","Pilot","HP" });
	param_.dropped = false;

	maxHp_ = param_.hp;
	droppedHeight_ = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","Pilot","DroppedHeight" });

}

void PilotMechStatus::Update() {
	// 衝突座標をリセット
	hitPos_ = { 0.0f,0.0f,0.0f };

	// 被弾を処理する
	ReactHitInfo();
	// 落下したかどうかを判定する
	JudgeDropped();

}

Vector3 PilotMechStatus::GetHitPos() const {
	return hitPos_;
}

int32_t PilotMechStatus::GetHP() const {
	return param_.hp;
}

int32_t PilotMechStatus::GetMaxHP() const {
	return maxHp_;
}

bool PilotMechStatus::GetIsDropped() const {
	return param_.dropped;
}

int32_t PilotMechStatus::GetJustDodgeStreak() const {
	return param_.justDodgeStreak;
}

int32_t PilotMechStatus::GetJustDodgeScore() const {
	return param_.justDodgeScore;
}

float PilotMechStatus::GetAttackMul() const {
	return param_.attackMul;
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

void PilotMechStatus::JudgeDropped() {
	// パイロットの高さを取得して、一定値以下なら落下フラグを立てる
	const float mechPosY = mech_->GetTransform()->GetWorldPosition().y;
	if (mechPosY <= droppedHeight_) {
		param_.dropped = true;
	} else {
		// いらなそうな処理だけど一応書いておく
		param_.dropped = false;
	}
}
