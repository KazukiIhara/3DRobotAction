#include "TPSCamera3D.h"

// C++
#include <algorithm>
#include <cmath>

#include "Framework/MAGI.h"
#include "3D/Transform3D/Transform3D.h"

// Math
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

namespace {
	float Clamp(float v, float a, float b) {
		return std::clamp(v, a, b);
	}

	Vector3 ExpSmoothing(const Vector3& current, const Vector3& target, float sharpness, float dt) {
		const float t = 1.0f - std::exp(-sharpness * dt);
		return Lerp(current, target, t);
	}
}

TPSCamera3D::TPSCamera3D(const std::string& name)
	: Camera3D(name, false) // isUseYawPitch=false で開始
{
	SetIsUseYawPitch(false);

	smoothedEye_ = GetEye();
	smoothedTarget_ = GetTarget();
}

void TPSCamera3D::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	if (!follow_) {
		UpdateData();
		return;
	}

	const Vector3 desiredTarget = CalcDesiredTarget();
	const Vector3 desiredEye = CalcDesiredEye(desiredTarget);

	// 平滑化
	smoothedTarget_ = ExpSmoothing(smoothedTarget_, desiredTarget, param_.targetSmooth, dt);
	smoothedEye_ = ExpSmoothing(smoothedEye_, desiredEye, param_.posSmooth, dt);

	// 反映
	SetTarget(smoothedTarget_);
	SetEye(smoothedEye_);


	UpdateData();
}

void TPSCamera3D::SetFollowTransform(Transform3D* follow) {
	follow_ = follow;

	if (follow_) {
		const Vector3 desiredTarget = CalcDesiredTarget();
		const Vector3 desiredEye = CalcDesiredEye(desiredTarget);

		smoothedTarget_ = desiredTarget;
		smoothedEye_ = desiredEye;

		SetTarget(smoothedTarget_);
		SetEye(smoothedEye_);
	}
}

void TPSCamera3D::SetLockOnTarget(Transform3D* lockOn) {
	lockOn_ = lockOn;
}

void TPSCamera3D::AddYaw(float delta) {
	SetYaw(GetYaw() + delta * param_.yawSpeed);
}

void TPSCamera3D::AddPitch(float delta) {
	float p = GetPitch() + delta * param_.pitchSpeed;
	p = Clamp(p, param_.pitchMin, param_.pitchMax);
	SetPitch(p);
}

TPSCamera3D::Param& TPSCamera3D::GetParam() {
	return param_;
}
const TPSCamera3D::Param& TPSCamera3D::GetParam() const {
	return param_;
}

Vector3 TPSCamera3D::CalcDesiredTarget() const {
	Vector3 base = follow_->GetWorldPosition();
	base.y += param_.height;

	// ロックオンがあるなら少し寄せる
	if (lockOn_) {
		const Vector3 lockPos = lockOn_->GetWorldPosition();
		base = Lerp(base, lockPos, 0.35f);
	}

	return base;
}

Vector3 TPSCamera3D::CalcDesiredEye(const Vector3& desiredTarget) const {
	// yaw/pitch から forward を作る
	const Vector3 fwd = DirectionFromYawPitch(GetYaw(), GetPitch());

	// ワールドUp固定
	const Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f };

	// right を作る
	Vector3 right = Normalize(Cross(up, fwd));

	// 肩越し + 距離
	const Vector3 shoulder = right * param_.shoulder;
	const Vector3 back = -Normalize(fwd) * param_.distance;

	// ほんの少しカメラ自体も上げる
	const Vector3 eyeUp = up * (param_.height * 0.15f);

	return desiredTarget + back + shoulder + eyeUp;
}
