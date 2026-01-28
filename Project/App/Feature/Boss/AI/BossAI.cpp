#include "BossAI.h"

// C++
#include <algorithm>
#include <cmath>

#include "MAGI.h"
#include "Feature/Boss/Mech/BossMech.h"
#include "Feature/Pilot/Mech/PilotMech.h"

using namespace Magi;

BossAI::BossAI(BossMech* mech) {
	mech_ = mech;
}

void BossAI::SetParam(const Param& param) {
	param_ = param;
}

void BossAI::Update(bool isActive, bool isPause) {
	if (!mech_ || !isActive || isPause) {
		return;
	}

	const float dt = MAGISYSTEM::GetDeltaTime();

	// クールダウン更新
	if (commitTimerSec_ > 0.0f) {
		commitTimerSec_ -= dt;
		commitTimerSec_ = std::max(0.0f, commitTimerSec_);
	}

	// クールダウン中は選択しない
	if (commitTimerSec_ > 0.0f) {
		return;
	}

	// Idle以外は遷移しない
	if (mech_->GetCurrentState() != BossMech::State::Idle) {
		return;
	}

	// 思考タイミング以外は何もしない
	if (!UpdateThinkTimer()) {
		return;
	}

	// 行動選択
	const Action action = SelectAction();

	// 行動適用
	ApplyAction(action);

	// 選択直後は少し待つ
	commitTimerSec_ = std::max(0.0f, param_.commitAfterSelectSec);
}

bool BossAI::UpdateThinkTimer() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	// タイマー更新
	thinkTimerSec_ -= dt;
	thinkTimerSec_ = std::max(0.0f, thinkTimerSec_);

	if (thinkTimerSec_ > 0.0f) {
		return false;
	}

	thinkTimerSec_ = std::max(0.01f, param_.thinkIntervalSec);
	return true;
}

BossAI::Action BossAI::SelectAction() {
	const float dist = CalcDistanceToPilot();

	ActionScore best{};
	best.action = Action::Idle;
	best.score = ScoreIdle(dist) + Jitter();

	{
		const float s = ScoreLaserShot(dist) + Jitter();
		if (s > best.score) {
			best = { Action::LaserShot, s };
		}
	}
	{
		const float s = ScoreLaserBladeSlash(dist) + Jitter();
		if (s > best.score) {
			best = { Action::LaserBladeSlash, s };
		}
	}

	return best.action;
}

float BossAI::CalcDistanceToPilot() const {
	auto* pilot = mech_->GetPilotMech();
	if (!pilot) {
		return 0.0f;
	}

	const Vector3 a = mech_->GetCenterPos();
	const Vector3 b = pilot->GetCenterPos();
	const Vector3 d = b - a;

	// 距離
	return Length(d);
}

float BossAI::ScoreIdle(float dist) const {
	const float mid = (param_.nearDist + param_.farDist) * 0.5f;
	const float denom = std::max(1.0f, mid);
	const float t = std::clamp(1.0f - std::abs(dist - mid) / denom, 0.0f, 1.0f);
	return 0.1f * t;
}

float BossAI::ScoreLaserShot(float dist) const {
	// 近すぎると不利
	if (dist < param_.nearDist) {
		return -1.0f;
	}

	// 遠いほど高い
	const float t = std::clamp(
		(dist - param_.nearDist) / std::max(0.01f, (param_.farDist - param_.nearDist)),
		0.0f, 1.0f
	);
	return 1.0f * t;
}

float BossAI::ScoreLaserBladeSlash(float dist) const {
	// 遠すぎると不利
	if (dist > param_.farDist) {
		return -1.0f;
	}

	// 近いほど高い
	const float t = std::clamp(1.0f - (dist / std::max(0.01f, param_.nearDist)), 0.0f, 1.0f);
	return 1.0f * t;
}

void BossAI::ApplyAction(Action action) {
	switch (action) {
		case Action::Idle:
			// そのまま待機
			break;

		case Action::LaserShot:
			mech_->ChangeState(BossMech::State::LaserShot);
			break;

		case Action::LaserBladeSlash:
			mech_->ChangeState(BossMech::State::LaserBladeSlash);
			break;

		default:
			break;
	}
}

float BossAI::Jitter() {
	const float r = dist01_(rng_);
	return (r - 0.5f) * 2.0f * param_.randomJitter;
}
