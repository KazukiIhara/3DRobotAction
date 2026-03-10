#include "PilotMechStatus.h"

#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

PilotMechStatus::PilotMechStatus(PilotMech* mech) {
	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "MechInitParam","Pilot","HP" }, ParamType::Int32);
	MAGISYSTEM::AddParameterData({ "MechInitParam","Pilot","DroppedHeight" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "MechInitParam","Pilot","ScoreDecayRate" }, ParamType::Float);
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
	// 攻撃倍率更新
	UpdateDodgeScoreSystem();
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

float PilotMechStatus::GetJustDodgeGauge() const {
	return param_.justDodgeGauge;
}

float PilotMechStatus::GetAttackMul() const {
	return param_.attackMul;
}

void PilotMechStatus::AddJustDodgeScore() {
	param_.justDodgeScore++;
}

void PilotMechStatus::AddJustDodgeStreak() {
	param_.justDodgeStreak++;
}

void PilotMechStatus::AddJustDodgeGauge() {
	param_.justDodgeGauge += 1.0f;
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

		// 被弾時処理

		// ジャスト回避ストリークをリセット
		param_.justDodgeStreak = 0;


	}


}

void PilotMechStatus::UpdateDodgeScoreSystem() {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// もし100を超えていたら
	if (param_.justDodgeGauge >= 1.0f) {
		// スコアを加算
		param_.justDodgeScore++;
		// 少しだけゲージを減らす
		param_.justDodgeGauge = 0.99f;
	}

	// 時間経過で減らす
	const float scoreDecayRate = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","Pilot","ScoreDecayRate" });
	param_.justDodgeGauge -= dt * scoreDecayRate;

	// もしゲージが0になっていればスコアをリセットする
	if (param_.justDodgeGauge <= 0.0f) {
		param_.justDodgeScore = 0;
	}

	// 1が上限
	param_.justDodgeGauge = std::clamp(param_.justDodgeGauge, 0.0f, 1.0f);

	// 0:D 1:C 2:B 3:A 4:S
	const int32_t score = param_.justDodgeScore;

	switch (score) {
	case 0:
		param_.attackMul = 1.0f;
		break;
	case 1:
		param_.attackMul = 1.5f;
		break;
	case 2:
		param_.attackMul = 2.0f;
		break;
	case 3:
		param_.attackMul = 2.5f;
		break;
	case 4:
		param_.attackMul = 3.0f;
		break;
	default:
		param_.attackMul = 3.0f;
		break;
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
