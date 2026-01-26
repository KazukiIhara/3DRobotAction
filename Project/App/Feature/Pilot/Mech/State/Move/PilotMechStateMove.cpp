#include "PilotMechStateMove.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateMove::Enter(PilotMech* mech) {
	auto ms = mech->GetMoveSystem();
	const float maxSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Move","MaxSpeed" });
	ms->SetMaxSpeed(maxSpeed);
}

void PilotMechStateMove::Update(PilotMech* mech) {
	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	// 移動入力なしでIdleに遷移
	if (commandPair.first) {
		auto command = commandPair.second;

		// 左手武器で攻撃
		if (command.attackL) {
			mech->GetWeapon("BeamCannonRifle")->Attack();
		}

		// 右手武器で攻撃
		if (command.attackR) {
			mech->GetWeapon("MachineGun")->Attack();
		}

		if (command.dodge) {
			mech->ChangeState(PilotMech::State::Dodge);
			return;
		}

		if (!Length(command.common.StickL)) {
			mech->ChangeState(PilotMech::State::Idle);
			return;
		}

		// 移動処理
		Camera3D* cuCamera = MAGISYSTEM::GetCurrentCamera3D();
		const Vector2 dir = StickToMoveDirXZ(command.common.StickL, cuCamera->GetEye(), cuCamera->GetTarget());
		const float acc = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Move","Acc" });
		const float maxSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Move","MaxSpeed" });
		// 移動システム
		auto ms = mech->GetMoveSystem();
		ms->SetDirXZ(dir);
		ms->SetAcc(acc);
		ms->SetMaxSpeed(maxSpeed);

		// 回転に補正を掛け　急な角度の移動量を減速する
		ms->ApplyRotationInertia();
	}

}

void PilotMechStateMove::Exit([[maybe_unused]] PilotMech* mech) {

}