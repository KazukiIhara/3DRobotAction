#include "PilotMechStateIdle.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"
#include "Feature/Pilot/Mech/System/Move/PilotMechMoveSystem.h"

#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void PilotMechStateIdle::Enter(PilotMech* mech) {

}

void PilotMechStateIdle::Update(PilotMech* mech) {
	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	// 移動入力でMoveに遷移
	if (commandPair.first) {
		auto command = commandPair.second;
		if (Length(command.common.StickL)) {
			mech->ChangeState(PilotMech::State::Move);
			return;
		}
	}

	// 摩擦による減速処理
	const float acc = -20.0f;
	const float maxSpeed = 20.0f;
	// 移動システム
	auto ms = mech->GetMoveSystem();
	ms->SetAcc(acc);
	ms->SetMaxSpeed(maxSpeed);
}

void PilotMechStateIdle::Exit(PilotMech* mech) {

}