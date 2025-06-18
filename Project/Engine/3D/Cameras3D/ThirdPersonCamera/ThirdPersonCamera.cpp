#include "ThirdPersonCamera.h"

#include "MAGI.h"

using namespace MAGIMath;

ThirdPersonCamera::ThirdPersonCamera()
	:Camera3D() {}

ThirdPersonCamera::~ThirdPersonCamera() {

}

void ThirdPersonCamera::Initialize() {
	Camera3D::Initialize();
	isUseYawPitch_ = false;
}

void ThirdPersonCamera::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();
	const float kLimit = DegreeToRadian(89.0f);
	if (!targetTransform_) return;

	// 入力処理
	if (MAGISYSTEM::IsPadConnected(0)) {
		yaw_ += MAGISYSTEM::GetRightStickX(0) * yawSpeed_ * dt;
		pitch_ += MAGISYSTEM::GetRightStickY(0) * pitchSpeed_ * dt;
	}
	pitch_ = std::clamp(pitch_, -kLimit, kLimit);

	// ピボット計算
	Vector3 rawPivot = targetTransform_->GetWorldPosition() + pivotOffset_;

	// 追従ラグ処理
	if (followLag_ <= 0.0f) {
		smoothedPivot_ = rawPivot;                      // 即追従
	} else {
		const float λ = 1.0f - std::exp(-dt / followLag_); 
		smoothedPivot_ += (rawPivot - smoothedPivot_) * λ;
	}

	// 眼位置計算
	Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
	Vector3 shoulder = Normalize(Cross(forward, { 0,1,0 })) * 1.5f * shoulderSign_; // ←任意
	eye_ = smoothedPivot_ - forward * radius_ + shoulder;

	target_ = smoothedPivot_;
	UpdateData();
}

void ThirdPersonCamera::SetTargetTransform(Transform3D* target) {
	targetTransform_ = target;
	smoothedPivot_ = targetTransform_->GetWorldPosition() + pivotOffset_;
}
