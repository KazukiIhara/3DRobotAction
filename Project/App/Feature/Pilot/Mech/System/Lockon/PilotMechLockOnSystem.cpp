#include "PilotMechLockOnSystem.h"

#include "Feature/Pilot/Mech/PilotMech.h"

PilotMechLockOnSystem::PilotMechLockOnSystem(PilotMech* mech) {
	mech_ = mech;

}

void PilotMechLockOnSystem::Update() {

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

void PilotMechLockOnSystem::SetBoss(ILockOnTarget* bossMech) {
	target_ = bossMech;
}
