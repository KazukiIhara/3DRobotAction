#include "ThirdPersonCamera.h"

#include "MAGI.h"

using namespace MAGIMath;

ThirdPersonCamera::ThirdPersonCamera(const std::string& name)
	:Camera3D(name, false) {}

ThirdPersonCamera::~ThirdPersonCamera() {

}

void ThirdPersonCamera::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	const float kMaxLimit = DegreeToRadian(maxPitchDegrees_);
	const float kMinLimit = DegreeToRadian(minPitchDegrees_);

	if (!followTargetTransform_) return;

	// 入力処理
	if (MAGISYSTEM::IsPadConnected(0)) {
		const Vector2 rStick = MAGISYSTEM::GetRightStick(0);
		yaw_ += rStick.x * yawSpeed_ * dt;
		pitch_ += rStick.y * pitchSpeed_ * dt;
	} else {
		yaw_ += MAGISYSTEM::GetMouseMoveDeltaX() * mouseYawSpeed_ * dt;
		pitch_ -= MAGISYSTEM::GetMouseMoveDeltaY() * mousePitchSpeed_ * dt;
	}

	pitch_ = std::clamp(pitch_, kMinLimit, kMaxLimit);

	// ピボット計算
	Vector3 rawPivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;

	// 水平と垂直を分離
	if (!followLagHorizontal_ && !followLagVertical_) {
		smoothedPivot_ = rawPivot;
	} else {
		const float tH =
			(followLagHorizontal_ <= 0.0f) ? 1.0f : 1.0f - std::exp(-dt / followLagHorizontal_);

		const float tV =
			(followLagVertical_ <= 0.0f) ? 1.0f : 1.0f - std::exp(-dt / followLagVertical_);

		// XZ（水平）
		smoothedPivot_.x += (rawPivot.x - smoothedPivot_.x) * tH;
		smoothedPivot_.z += (rawPivot.z - smoothedPivot_.z) * tH;
		// Y（垂直）
		smoothedPivot_.y += (rawPivot.y - smoothedPivot_.y) * tV;
	}

	// 位置計算
	Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
	Vector3 shoulder = Normalize(Cross(forward, { 0,1,0 })) * 1.5f * shoulderSign_; // ←任意
	eye_ = smoothedPivot_ - forward * radius_ + shoulder;

	target_ = smoothedPivot_;
	UpdateData();
}

void ThirdPersonCamera::SetTargetTransform(Transform3D* target) {
	followTargetTransform_ = target;
	smoothedPivot_ = followTargetTransform_->GetWorldPosition() + pivotOffset_;
}
