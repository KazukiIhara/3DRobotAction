#include "PilotMechMoveSystem.h"

#include "Math/Utility/MathUtility.h"
#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

PilotMechMoveSystem::PilotMechMoveSystem(PilotMech* mech) {
	mech_ = mech;

	dir_ = { 0.0f,0.0f,1.0f };
	acc_ = 0.0f;
	speed_ = 0.0f;
	maxSpeed_ = 0.0f;

	velocity_ = dir_ * speed_;
}

void PilotMechMoveSystem::Update() {
	// 移動速度を計算
	speed_ += acc_ * MAGISYSTEM::GetDeltaTime();

	// 最大速度を制限
	speed_ = std::min(maxSpeed_, speed_);

	// 移動量計算
	velocity_ = dir_ * speed_ * MAGISYSTEM::GetDeltaTime();

	mech_->GetTransform()->AddTranslate(velocity_);
}

void PilotMechMoveSystem::SetDir(const Vector3& dir) {
	dir_ = dir;
}

void PilotMechMoveSystem::SetAcc(float acc) {
	acc_ = acc;
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
