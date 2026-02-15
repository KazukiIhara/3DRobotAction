#include "MechMoveSystem.h"

#include "Math/Utility/MathUtility.h"
#include "Feature/Mech/Base/BaseMech.h"

#include "MAGI.h"

using namespace Magi;

MechMoveSystem::MechMoveSystem(BaseMech* mech) {
	mech_ = mech;

	dir_ = { 0.0f,0.0f,1.0f };
	preDir_ = dir_;
	acc_ = 0.0f;
	speed_ = 0.0f;
	maxSpeed_ = 0.0f;
	verticalSpeed_ = 0.0f;

	velocity_ = dir_ * speed_;
}

void MechMoveSystem::Update() {
	CalSpeed();
	ApplyVelocity();
}

void MechMoveSystem::SetDir(const Vector3& dir) {
	dir_ = dir;
}

void MechMoveSystem::SetDirY(float dirY) {
	dir_.y = dirY;
}

void MechMoveSystem::SetDirXZ(const Vector2& dirXZ) {
	dir_.x = dirXZ.x;
	dir_.z = dirXZ.y;
}

void MechMoveSystem::SetAcc(float acc) {
	acc_ = acc;
}

void MechMoveSystem::SetSpeed(float speed) {
	speed_ = speed;
}

void MechMoveSystem::SetMaxSpeed(float maxSpeed) {
	maxSpeed_ = maxSpeed;
}

float MechMoveSystem::GetSpeed() const {
	return speed_;
}

const Vector3& MechMoveSystem::GetDir() const {
	return dir_;
}

const Vector3& MechMoveSystem::GetVelocity() const {
	return velocity_;
}

void MechMoveSystem::SetVerticalSpeed(float vertical) {
	// 垂直速度
	verticalSpeed_ = vertical;
}

float MechMoveSystem::GetVerticalSpeed() const {
	return verticalSpeed_;
}

void MechMoveSystem::Reset() {
	dir_ = { 0.0f,0.0f,1.0f };
	preDir_ = dir_;
	acc_ = 0.0f;
	speed_ = 0.0f;
	maxSpeed_ = 0.0f;
	verticalSpeed_ = 0.0f;
}

void MechMoveSystem::ApplyRotationInertia() {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 角度差を求める
	float dot = Dot(preDir_, dir_);
	float expT = CalExpT(dt, 0.1f, 1.0f);
	// 角度差が大きい場合減速する
	if (dot < -0.1f) {
		speed_ = Lerp(speed_, 0.0f, expT);
	}

	const float clampedDot = -(dot + 1.0f) * 0.5f;
	const float targetSec = std::max(5.0f, clampedDot * 15.0f);

	// 方向を正規化
	dir_ = Normalize(dir_);
	const float t = CalExpT(dt, targetSec, 1.0f);
	// 移動方向を補間
	dir_ = Lerp(preDir_, dir_, t);
}

void MechMoveSystem::CalSpeed() {
	// dt取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 移動速度を計算
	speed_ += acc_ * dt;
	speed_ = std::max(0.0f, speed_);

	// 速度を補間
	if (speed_ > maxSpeed_) {
		const float t = CalExpT(dt, 2.0f, 1.0f);
		speed_ = Lerp(speed_, maxSpeed_, t);
	}

}

void MechMoveSystem::ApplyVelocity() {
	// dt取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 水平速度
	velocity_ = Normalize(dir_) * speed_;

	// 垂直速度を合成
	velocity_.y += verticalSpeed_;

	// このフレームの移動角度を保存
	preDir_ = dir_;

	// 機体を動かす
	mech_->GetTransform()->AddTranslate(velocity_ * dt);
}