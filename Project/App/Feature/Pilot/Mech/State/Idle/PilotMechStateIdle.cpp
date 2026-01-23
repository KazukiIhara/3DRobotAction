#include "PilotMechStateIdle.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;


void PilotMechStateIdle::Enter([[maybe_unused]] PilotMech* mech) {

}

void PilotMechStateIdle::Update(PilotMech* mech) {
	// 摩擦による減速処理
	const float acc = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Idle","Acc" });
	const float maxSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Idle","MaxSpeed" });

	// 移動システム
	auto ms = mech->GetMoveSystem();
	ms->SetAcc(acc);
	ms->SetMaxSpeed(maxSpeed);

	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	// 移動入力でMoveに遷移
	if (commandPair.first) {
		auto command = commandPair.second;
		if (command.dodge) {
			mech->ChangeState(PilotMech::State::Dodge);
			return;
		}
		if (Length(command.common.StickL)) {
			mech->ChangeState(PilotMech::State::Move);
			return;
		}
	}
}

void PilotMechStateIdle::Exit([[maybe_unused]] PilotMech* mech) {

}