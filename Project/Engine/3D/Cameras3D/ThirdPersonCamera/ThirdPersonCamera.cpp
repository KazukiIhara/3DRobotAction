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
	const float kLimit = DegreeToRadian(89.0f);

	if (!targetTransform_)return;

	// パッド入力
	if (MAGISYSTEM::IsPadConnected(0)) {
		yaw_ += MAGISYSTEM::GetRightStickX(0) * dt;
		pitch_ -= MAGISYSTEM::GetRightStickY(0) * dt;
	}

	// ジンバルロック対策
	pitch_ = std::clamp(pitch_, -kLimit, kLimit);

	// 希望カメラ位置計算
	Vector3 pivot = targetTransform_->GetWorldPosition() + pivotOffset_;
	Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
	Vector3 shoulder = Normalize(Cross(forward, { 0,1,0 })) * 0.4f * shoulderSign_;
	Vector3 desiredEye = pivot - forward * radius_ + shoulder;

	eye_ += (desiredEye - eye_) * (1.0f - std::exp(-followLag_ * dt));
	target_ = pivot;

	UpdateData();
}

void ThirdPersonCamera::SetTargetTransform(Transform3D* target) {
	targetTransform_ = target;
}
