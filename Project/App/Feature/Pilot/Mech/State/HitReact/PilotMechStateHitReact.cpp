#include "PilotMechStateHitReact.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateHitReact::Enter([[maybe_unused]] PilotMech* mech) {
	// ひとまず固定値のヒットリアクション
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","HitReact","Time" });

	// 被弾時アニメーション再生

}

void PilotMechStateHitReact::Update([[maybe_unused]] PilotMech* mech) {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	// タイマー更新
	timer_ -= dt;
	timer_ = std::min(0.0f, timer_);
	if (timer_ <= 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
	}
}

void PilotMechStateHitReact::Exit([[maybe_unused]] PilotMech* mech) {

}
