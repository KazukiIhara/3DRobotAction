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
	maxSpeed_ = 0.0f;

	velocity_ = dir_ * speed_;
}

void PilotMechMoveSystem::Update() {
	// dt取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 移動速度を計算
	speed_ += acc_ * dt;

	// 速度をクランプ
	speed_ = std::clamp(speed_, 0.0f, maxSpeed_);

	// 方向を正規化
	dir_ = Normalize(dir_);

	// 移動方向の差によって減速させる
	TurnDeceleration(dt);

	// 移動量計算
	velocity_ = dir_ * speed_;

	// このフレームの移動角度を保存
	preDir_ = dir_;

	// 機体を動かす
	mech_->GetTransform()->AddTranslate(velocity_ * MAGISYSTEM::GetDeltaTime());

	// 機体を進行方向に向ける
	TurnToDirection();
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

const Vector3& PilotMechMoveSystem::GetDir() const {
	return dir_;
}

const Vector3& PilotMechMoveSystem::GetVelocity() const {
	return velocity_;
}

void PilotMechMoveSystem::TurnToDirection() {

	const Vector2 currentDir = Vector2(dir_.x, dir_.z);
	const float yaw = std::atan2(currentDir.x, currentDir.y);
	const Quaternion targetQ = MakeRotateAxisAngleQuaternion({ 0.0f,1.0f,0.0f }, yaw);

	mech_->GetDrawTransform()->SetQuaternion(targetQ);
}

void PilotMechMoveSystem::TurnDeceleration(float dt) {
	// 前方向が無効なら初期化
	Vector3 preN = preDir_;
	if (LengthSquared(preN) <= 1.0e-8f) {
		preN = dir_;
	} else {
		preN = Normalize(preN);
	}
	// 角度差を求める
	float dot = Dot(preN, dir_);
	if (dot < 0.0f) {
		speed_ = 0.5f;
	}
}