#define NOMINMAX

#include "MechCoreStateJustDodge.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"

using namespace Magi;

MechCoreStateJustDodge::MechCoreStateJustDodge() {

}

void MechCoreStateJustDodge::Enter([[maybe_unused]] MechCore* mechCore) {
	// タイマーをセット
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "MechCommonParam","JustDodge","StateTime" });

}

void MechCoreStateJustDodge::Update([[maybe_unused]] MechCore* mechCore) {
	// 関数内の定数
	const float kStateTime = MAGISYSTEM::GetParameterValue<float>({ "MechCommonParam","JustDodge","StateTime" });
	const float deltaMulStart = MAGISYSTEM::GetParameterValue<float>({ "MechCommonParam","JustDodge","DeltaMultiplierStart" });
	const float deltaMulEnd = MAGISYSTEM::GetParameterValue<float>({ "MechCommonParam","JustDodge","DeltaMultiplierEnd" });
	SimpleAnimation<float> deltaTimeAnim = SimpleAnimation<float>(deltaMulStart, deltaMulEnd, EasingType::EaseInQuart);

	// タイマーを更新(生デルタタイムを使う)
	timer_ -= MAGISYSTEM::GetRawDeltaTime();
	timer_ = std::max(0.0f, timer_);
	const float t = 1.0f - timer_ / kStateTime;

	// デルタタイムを遅くする
	const float deltaMul = deltaTimeAnim.GetValue(t);

	MAGISYSTEM::SetDeltaTimeMultiplier(deltaMul);

	// 終了したら通常ステートに戻す
	if (timer_ == 0.0f) {
		mechCore->ChangeState(MechCoreState::Move);
	}

}

void MechCoreStateJustDodge::Exit([[maybe_unused]] MechCore* mechCore) {

}
