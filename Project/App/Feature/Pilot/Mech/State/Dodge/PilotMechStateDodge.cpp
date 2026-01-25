#include "PilotMechStateDodge.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateDodge::Enter(PilotMech* mech) {
	// タイマー
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","Time" });
	// 初速
	const float firstSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","FirstSpeed" });

	// ステートのタイマーセット
	timer_ = time;

	// 加速量を0にする
	auto ms = mech->GetMoveSystem();
	ms->SetAcc(0.0f);
	// 初速と最大速度をセット
	ms->SetSpeed(firstSpeed);
	ms->SetMaxSpeed(firstSpeed);

	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	if (commandPair.first) {
		auto command = commandPair.second;
		Camera3D* cuCamera = MAGISYSTEM::GetCurrentCamera3D();
		Vector2 dir{};
		// 入力がない場合は前方に回避
		if (Length(command.common.StickL)) {
			dir = command.common.StickL; ;
		} else {
			dir = { 0.0f,1.0f };
		}
		// カメラの向きを前方に変換
		dir = StickToMoveDirXZ(dir, cuCamera->GetEye(), cuCamera->GetTarget());
		ms->SetDirXZ(dir);

	}


}

void PilotMechStateDodge::Update(PilotMech* mech) {
	// デルタタイム更新
	const float dt = MAGISYSTEM::GetDeltaTime();
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","Time" });

	// ジャスト回避判定
	if (mech->GetJustDodgeCollider()->GetIsHit()) {
		mech->ChangeState(PilotMech::State::JustDodge);
		return;
	}

	// 移動処理
	{
		auto ms = mech->GetMoveSystem();
		const float t = CalExpT(dt, time, 1.0f);
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
		return;
	}
}

void PilotMechStateDodge::Exit([[maybe_unused]] PilotMech* mech) {

}