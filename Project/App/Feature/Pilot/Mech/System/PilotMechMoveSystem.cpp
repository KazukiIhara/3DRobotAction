#include "PilotMechMoveSystem.h"

#include "Math/Utility/MathUtility.h"
#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

PilotMechMoveSystem::PilotMechMoveSystem(PilotMech* mech) {
	mech_ = mech;

	dir_ = { 0.0f,0.0f,1.0f };
	speed_ = 0.0f;
	velocity_ = dir_ * speed_;
	maxSpeed_ = 0.0f;
}

void PilotMechMoveSystem::Update() {
	// 移動速度を制限(0.0の場合は制限しない)
	if (maxSpeed_ != 0.0) {
		speed_ = std::min(maxSpeed_, speed_);
	}
	// 移動量計算
	velocity_ = dir_ * speed_ * MAGISYSTEM::GetDeltaTime();

	mech_->GetTransform()->AddTranslate(velocity_);
}

void PilotMechMoveSystem::SetDir(const Vector3& dir) {
	dir_ = dir;
}

void PilotMechMoveSystem::SetSpeed(float speed) {
	speed_ = speed;
}

void PilotMechMoveSystem::SetMaxSpeed(float maxSpeed) {
	maxSpeed_ = maxSpeed;
}

const Vector3 PilotMechMoveSystem::GetVelocity() const {
	return velocity_;
}
