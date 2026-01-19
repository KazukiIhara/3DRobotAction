#include "PilotMechStateDodge.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"
#include "Feature/Pilot/Mech/System/Move/PilotMechMoveSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateDodge::Enter(PilotMech* mech) {
	// タイマー
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","Time" });
	// ステートのタイマーセット
	timer_ = time;

	// 初速
	const float firstSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","FirstSpeed" });

	// 加速量を0にする
	auto ms = mech->GetMoveSystem();
	ms->SetAcc(0.0f);

	// 初速と最大速度をセット
	ms->SetSpeed(firstSpeed);
	ms->SetMaxSpeed(firstSpeed);
}

void PilotMechStateDodge::Update(PilotMech* mech) {
	const float dt = MAGISYSTEM::GetDeltaTime();
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","Time" });
	auto ms = mech->GetMoveSystem();

	{
		const float t = CalExpT(dt, time, 0.5f);
		// 通常移動の最大速度まで補間
		const float maxMoveSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Move","MaxSpeed" });
		const float currentSpeed = ms->GetSpeed();
		const float speed = Lerp(currentSpeed, maxMoveSpeed, t);
		ms->SetSpeed(speed);
	}

	// タイマー更新
	timer_ -= MAGISYSTEM::GetDeltaTime();
	timer_ = std::max(0.0f, timer_);
	// タイマー終了で移動状態に遷移
	if (timer_ == 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
	}
}

void PilotMechStateDodge::Exit([[maybe_unused]] PilotMech* mech) {}