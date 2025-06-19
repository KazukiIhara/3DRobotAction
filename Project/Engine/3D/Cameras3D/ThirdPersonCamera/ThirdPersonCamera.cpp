#include "ThirdPersonCamera.h"

#include "MAGI.h"

using namespace MAGIMath;

ThirdPersonCamera::ThirdPersonCamera()
	:Camera3D() {
}

ThirdPersonCamera::~ThirdPersonCamera() {

}

void ThirdPersonCamera::Initialize() {
	Camera3D::Initialize();
	isUseYawPitch_ = false;
}

void ThirdPersonCamera::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	const float kMaxLimit = DegreeToRadian(maxPitchDegrees_);
	const float kMinLimit = DegreeToRadian(minPitchDegrees_);

	if (!followTargetTransform_) return;

	// 入力処理
	if (MAGISYSTEM::IsPadConnected(0)) {
		yaw_ += MAGISYSTEM::GetRightStickX(0) * yawSpeed_ * dt;
		pitch_ += MAGISYSTEM::GetRightStickY(0) * pitchSpeed_ * dt;
	}
	pitch_ = std::clamp(pitch_, kMinLimit, kMaxLimit);

	// ピボット計算
	Vector3 rawPivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;

	// 追従ラグ処理
	if (followLag_ <= 0.0f) {
		smoothedPivot_ = rawPivot;                      // 即追従
	} else {
		const float t = 1.0f - std::exp(-dt / followLag_);
		smoothedPivot_ += (rawPivot - smoothedPivot_) * t;
	}

	// 眼位置計算
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
