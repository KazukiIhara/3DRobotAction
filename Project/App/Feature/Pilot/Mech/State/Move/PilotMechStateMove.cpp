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
	// アニメーション適用
	mech->GetAnimator()->ApproachPose("Pilot_Move", 0.8f);
}

void PilotMechStateMove::Update(PilotMech* mech) {
	// アニメーション適用
	mech->GetAnimator()->ApproachPose("Pilot_Move", 0.8f);
	// 右手武器攻撃可能フラグをTrueにする
	mech->GetCombatActionSystem()->SetEnableRightWeapon(true);

	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	// 移動入力なしでIdleに遷移
	if (commandPair.first) {
		auto command = commandPair.second;
		// 回避
		if (command.dodge) {
			mech->ChangeState(PilotMech::State::Dodge);
			return;
		}
		// スティック入力がなければ待機状態に遷移
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