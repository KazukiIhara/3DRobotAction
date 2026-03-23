#include "PilotMechLockOnSystem.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"

using namespace Magi;
using namespace MAGIMath;

PilotMechLockOnSystem::PilotMechLockOnSystem(PilotMech* mech) {
	mech_ = mech;

}

void PilotMechLockOnSystem::Update() {
	// 入力取得
	auto commandPair = mech_->GetInputSys()->GetPilotCommand();
	// 移動入力でMoveに遷移
	if (commandPair.first) {
		auto command = commandPair.second;
		if (command.switchLockOn) {
			SwitchLockOnFlag();
		}
	}


	// ロックオン対象がいて、ロックオンフラグがオンならTargetWorldPositionに値を入れる
	auto& flag = mech_->GetRotControlSystem()->GetLookAtFlag();
	if (target_ && isLockOn_) {
		mech_->SetTargetWorldPos(target_->GetCenterPos());
		// ロックオンフラグがオンなら機体のPitchをターゲットの方向に向ける (切りたい場合はステートで切る)
		flag.mech.yaw = true;
	} else {
		flag.mech.yaw = false;
		// ない場合はカメラの向きからある程度先の場所をターゲットにしてセットする
		// 現在カメラ取得
		Camera3D* cam = MAGISYSTEM::GetCurrentCamera3D();
		if (cam) {
			// カメラのForwardを取得
			Vector3 forward = cam->GetCameraVector().forward;
			// 念のため正規化
			forward = Normalize(forward);

			// カメラ位置から少し先をターゲットにする
			const Vector3 target = cam->GetEye() + forward * 100.0f;
			mech_->SetTargetWorldPos(target);
		}

	}
}

ILockOnTarget* PilotMechLockOnSystem::GetTarget() {
	return target_;
}

bool PilotMechLockOnSystem::GetIsLockOn() const {
	return isLockOn_;
}

void PilotMechLockOnSystem::SetBoss(ILockOnTarget* bossMech) {
	target_ = bossMech;
}

void PilotMechLockOnSystem::SwitchLockOnFlag() {
	isLockOn_ = !isLockOn_;
}
