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
		} else { // ハードロックオンオフ
			FollowCamera(dt);
		}
	}

	// カメラデータ更新
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
	// ターゲット座標取得
	Vector3 targetWorldPos{};
	if (auto core = core_.lock()) {
		if (auto tgt = core->GetLockOnComponent()->GetLockOnTarget().lock()) {
			if (auto obj = tgt->GetMechBody()->GetGameObject().lock()) {
				targetWorldPos = obj->GetTransform()->GetWorldPosition();
			}
		}
	}

	// ピボット平滑化
	Vector3 rawPivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;
	if (!followLagHorizontal_ && !followLagVertical_) {
		smoothedPivot_ = rawPivot;
	} else {
		const float tH = followLagHorizontal_ <= 0.0f ? 1.0f : 1.0f - std::exp(-dt / followLagHorizontal_);

		const float tV = followLagVertical_ <= 0.0f ? 1.0f : 1.0f - std::exp(-dt / followLagVertical_);
		smoothedPivot_.x += (rawPivot.x - smoothedPivot_.x) * tH;
		smoothedPivot_.z += (rawPivot.z - smoothedPivot_.z) * tH;
		smoothedPivot_.y += (rawPivot.y - smoothedPivot_.y) * tV;
	}

	// 角度追従
	Vector3 toTarget = Normalize(
		LengthSquared(targetWorldPos - smoothedPivot_) < 1e-6f
		? Vector3{ 0, 0, 1 } : targetWorldPos - smoothedPivot_
	);

	const float targetYaw = std::atan2(toTarget.x, toTarget.z);
	const float targetPitch = std::atan2(-toTarget.y, std::sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z));

	const float tYaw = rotLagYaw_ <= 0.0f ?
		1.0f : 1.0f - std::exp(-dt / rotLagYaw_);
	const float tPitch = rotLagPitch_ <= 0.0f ?
		1.0f : 1.0f - std::exp(-dt / rotLagPitch_);

	yaw_ = LerpAngle(yaw_, targetYaw, tYaw);
	pitch_ = Lerp(pitch_, targetPitch, tPitch);
	pitch_ = std::clamp(pitch_, DegreeToRadian(minPitchDegrees_), DegreeToRadian(maxPitchDegrees_));

	// カメラベクトル計算
	Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
	Vector3 shoulder = Normalize(Cross(forward, { 0, 1, 0 })) * 1.5f * shoulderSign_;

	// eye 位置
	Vector3 eye = smoothedPivot_ - forward * radius_ + shoulder;
	const float minY = smoothedPivot_.y + minBoomHeight_;
	if (eye.y < minY) {
		eye.y = minY;
		forward = Normalize((smoothedPivot_ + (targetWorldPos - smoothedPivot_) * focusBias_) - eye);
		shoulder = Normalize(Cross(forward, { 0, 1, 0 })) * 1.5f * shoulderSign_;
		eye = smoothedPivot_ - forward * radius_ + shoulder;
	}

	// 書き出し
	eye_ = eye;
	target_ = smoothedPivot_ + (targetWorldPos - smoothedPivot_) * focusBias_;
}

void PlayerCamera::FollowCamera(float dt) {
	// ピボット計算
	Vector3 rawPivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;

	// 水平と垂直を分離
	if (!followLagHorizontal_ && !followLagVertical_) {
		smoothedPivot_ = rawPivot;
	} else {
		const float tH = (followLagHorizontal_ <= 0.0f) ? 1.0f : 1.0f - std::exp(-dt / followLagHorizontal_);

		const float tV = (followLagVertical_ <= 0.0f) ? 1.0f : 1.0f - std::exp(-dt / followLagVertical_);

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

	if (eye_.y <= 0.3f) {
		eye_.y = 0.3f;
	}

	target_ = smoothedPivot_;
}
