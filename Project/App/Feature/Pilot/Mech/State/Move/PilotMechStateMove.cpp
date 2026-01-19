#include "PilotMechStateMove.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"
#include "Feature/Pilot/Mech/System/PilotMechMoveSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateMove::Enter(PilotMech* mech) {

}

void PilotMechStateMove::Update(PilotMech* mech) {
	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	// 移動入力なしでIdleに遷移
	if (commandPair.first) {
		auto command = commandPair.second;
		if (!Length(command.common.StickL)) {
			mech->ChangeState(PilotMech::State::Move);
			return;
		}

		Camera3D* cuCamera = MAGISYSTEM::GetCurrentCamera3D();
		const Vector2 dir = StickToMoveDirXZ(command.common.StickL, cuCamera->GetEye(), cuCamera->GetTarget());
		const float acc = 15.0f;
		const float maxSpeed = 20.0f;
		// 移動システム
		auto ms = mech->GetMoveSystem();
		ms->SetDirXZ(dir);
		ms->SetAcc(acc);
		ms->SetMaxSpeed(maxSpeed);


	}

}

void PilotMechStateMove::Exit(PilotMech* mech) {

}
