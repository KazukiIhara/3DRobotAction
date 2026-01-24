#include "PilotMechLockOnSystem.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

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

	// ロックオン対象がいたらTargetWorldPositionに値を入れる
	if (target_) {
		mech_->SetTargetWorldPos(target_->GetCenterPos());
	} else {
		// ない場合はカメラの向きからある程度先の場所をターゲットにしてセットする
		// 一旦原点
		mech_->SetTargetWorldPos(Vector3{});
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
