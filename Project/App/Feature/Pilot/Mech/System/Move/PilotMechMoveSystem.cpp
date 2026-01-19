#include "PilotMechMoveSystem.h"

#include "Math/Utility/MathUtility.h"
#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

PilotMechMoveSystem::PilotMechMoveSystem(PilotMech* mech) {
	mech_ = mech;

	dir_ = { 0.0f,0.0f,1.0f };
	preDir_ = dir_;
	acc_ = 0.0f;
	speed_ = 0.0f;
	maxSpeed_ = 10.0f;

	velocity_ = dir_ * speed_;
}

void PilotMechMoveSystem::Update() {
	// dt取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 移動速度を計算
	speed_ += acc_ * dt;
	speed_ = std::max(0.0f, speed_);

	// 速度を補間
	if (speed_ > maxSpeed_) {
		const float t = CalExpT(dt, 1.0f, 1.0f);
		speed_ = Lerp(speed_, maxSpeed_, t);
	}

	// 移動方向の差によって減速させる
	TurnDeceleration(dt);

	PilotMech::State current = mech_->GetCurrentState();
	if (current == PilotMech::State::Move) {
		// 方向を正規化
		dir_ = Normalize(dir_);
		const float t = CalExpT(dt, 4.0f, 1.0f);
		dir_ = Lerp(preDir_, dir_, t);
	}

	// 移動量計算
	velocity_ = dir_ * speed_;

	// このフレームの移動角度を保存
	preDir_ = dir_;

	// 機体を動かす
	mech_->GetTransform()->AddTranslate(velocity_ * dt);
}

void PilotMechMoveSystem::SetDir(const Vector3& dir) {
	dir_ = dir;
}

void PilotMechMoveSystem::SetDirXZ(const Vector2& dirXZ) {
	dir_.x = dirXZ.x;
	dir_.z = dirXZ.y;
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

float PilotMechMoveSystem::GetSpeed() const {
	return speed_;
}

const Vector3& PilotMechMoveSystem::GetDir() const {
	return dir_;
}

const Vector3& PilotMechMoveSystem::GetVelocity() const {
	return velocity_;
}

void PilotMechMoveSystem::TurnDeceleration(float dt) {
	// 角度差を求める
	float dot = Dot(preDir_, dir_);
	// 角度差が大きい場合速度を小さくする
	if (dot < -0.5f) {
		speed_ = 0.5f;
	}
}
