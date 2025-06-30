#include "PlayerCamera.h"

#include "MAGI.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

using namespace MAGIMath;

PlayerCamera::PlayerCamera(const std::string& name)
	:Camera3D(name, false) {

}

PlayerCamera::~PlayerCamera() {

}

void PlayerCamera::Update() {
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


	if (auto core = core_.lock()) {
		// ハードロックオンオフフラグ取得
		if (core->GetLockOnComponent()->GetEnableHardLockOn()) {
			if (core->GetLockOnComponent()->GetLockOnTarget().lock()) { // ターゲットあり
				HardLockOnCamera(dt);
			} else { // ターゲットなし
				FollowCamera(dt);
			}
		} else {
			FollowCamera(dt);
		}
	}

	UpdateData();
}

void PlayerCamera::SetTargetTransform(Transform3D* target) {
	followTargetTransform_ = target;
	smoothedPivot_ = followTargetTransform_->GetWorldPosition() + pivotOffset_;
}

void PlayerCamera::SetMechCore(std::weak_ptr<MechCore> mechCore) {
	core_ = mechCore;
}

void PlayerCamera::HardLockOnCamera(float dt) {
	// ロックオン対象のワールド座標を取得
	Vector3 targetWorldPos{};
	if (auto core = core_.lock()) {
		if (auto tgt = core->GetLockOnComponent()->GetLockOnTarget().lock()) {
			if (auto obj = tgt->GetMechBody()->GetGameObject().lock()) {
				targetWorldPos = obj->GetTransform()->GetWorldPosition();
			}
		}
	}


	Vector3 rawPivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;
	if (!followLagHorizontal_ && !followLagVertical_) {
		smoothedPivot_ = rawPivot;
	} else {
		const float tH = (followLagHorizontal_ <= 0.0f)
			? 1.0f
			: 1.0f - std::exp(-dt / followLagHorizontal_);
		const float tV = (followLagVertical_ <= 0.0f)
			? 1.0f
			: 1.0f - std::exp(-dt / followLagVertical_);

		// 水平(XZ)
		smoothedPivot_.x += (rawPivot.x - smoothedPivot_.x) * tH;
		smoothedPivot_.z += (rawPivot.z - smoothedPivot_.z) * tH;
		// 垂直(Y)
		smoothedPivot_.y += (rawPivot.y - smoothedPivot_.y) * tV;
	}

	// ターゲット方向から目標 Yaw/Pitch を算出
	Vector3 dir = targetWorldPos - smoothedPivot_;
	if (LengthSquared(dir) < 1e-6f) dir = { 0, 0, 1 };
	dir = Normalize(dir);
	const float targetYaw = std::atan2(dir.x, dir.z);
	const float targetPitch = std::atan2(-dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));

	// 現在の Yaw/Pitch を滑らかに追従
	const float tYaw = (rotLagYaw_ <= 0.f) ? 1.f : 1.f - std::exp(-dt / rotLagYaw_);
	const float tPitch = (rotLagPitch_ <= 0.f) ? 1.f : 1.f - std::exp(-dt / rotLagPitch_);
	yaw_ = LerpAngle(yaw_, targetYaw, tYaw);
	pitch_ = Lerp(pitch_, targetPitch, tPitch);

	// 制限角度へクランプ
	const float kMax = DegreeToRadian(maxPitchDegrees_);
	const float kMin = DegreeToRadian(minPitchDegrees_);
	pitch_ = std::clamp(pitch_, kMin, kMax);


	// カメラ位置を計算
	Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
	Vector3 shoulder = Normalize(Cross(forward, { 0,1,0 })) * 1.5f * shoulderSign_;

	eye_ = smoothedPivot_ - forward * radius_ + shoulder;

	Vector3 focus = smoothedPivot_ + (targetWorldPos - smoothedPivot_) * focusBias_;
	target_ = focus;
}

void PlayerCamera::FollowCamera(float dt) {
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
}
