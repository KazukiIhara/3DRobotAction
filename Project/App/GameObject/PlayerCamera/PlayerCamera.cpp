#include "PlayerCamera.h"

#include "MAGI.h"

#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

PlayerCamera::PlayerCamera(const std::string& name)
	:Camera3D(name, false) {

}

void PlayerCamera::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	if (!followTargetTransform_) return;

	// 入力を反省
	ApplyInput(dt);

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
}

void PlayerCamera::SetMechCore(std::weak_ptr<MechCore> mechCore) {
	core_ = mechCore;
}

void PlayerCamera::ApplyInput(float dt) {
	// ① 右スティック入力
	Vector2 rs{};
	if (MAGISYSTEM::IsPadConnected(0))
		rs = MAGISYSTEM::GetRightStick(0);
	if (LengthSquared(rs) < 1e-6f) return;

	// ② 入力を角速度へ
	yaw_ += rs.x * sensYaw_ * dt;				// Yaw : +右
	pitch_ -= rs.y * sensPitch_ * dt;           // Pitch: +上（符号反転）

	// ③ 角度域の正規化
	yaw_ = WrapPi(yaw_);                         // [-π, π)
	pitch_ = std::clamp(pitch_, -kPitchLim_, kPitchLim_);

	// ④ クォータニオン合成（Yaw → Pitch 順）
	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), yaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pitch_);

	boomRot_ = Normalize(qYaw * qPitch);
}

void PlayerCamera::HardLockOnCamera(float dt) {
	//----------------------------------------
	// ① ターゲット座標取得
	Vector3 targetWorldPos{};
	if (auto core = core_.lock()) {
		if (auto tgt = core->GetLockOnComponent()->GetLockOnTarget().lock()) {
			if (auto obj = tgt->GetMechBody()->GetGameObject().lock()) {
				targetWorldPos = obj->GetTransform()->GetWorldPosition();
			}
		}
	}

	//----------------------------------------
	// ② ピボットとターゲット方向
	const Vector3 pivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;
	Vector3 toTarget = Normalize(targetWorldPos - pivot);
	if (!Length(toTarget)) toTarget = MakeForwardVector3();

	//----------------------------------------
	// ③ 目標 Yaw / Pitch 角を算出
	float targetYaw = std::atan2(toTarget.x, toTarget.z);							// LH(+Z前)
	float targetPitch = std::asin(std::clamp(toTarget.y, -1.0f, 1.0f));
	targetPitch = std::clamp(targetPitch, -kPitchLim_, kPitchLim_);                 // 上下 ±80°

	//----------------------------------------
	// ④ スムーズ係数
	float t = 1.0f - std::exp(-dt / kHardLockLag_);                                 

	//----------------------------------------
	// ⑤ 角度を補間して反映
	yaw_ = LerpAngle(yaw_, targetYaw, t);
	pitch_ = LerpAngle(pitch_, targetPitch, t);

	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), yaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pitch_);
	boomRot_ = Normalize(qYaw * qPitch);

	//----------------------------------------
	// ⑥ カメラ位置補間
	Vector3 forward = Normalize(Transform(MakeForwardVector3(), boomRot_));
	Vector3 targetEye = pivot - forward * radius_;

	eye_ = Lerp(eye_, targetEye, t);
	target_ = Lerp(target_, targetWorldPos, t);
}

void PlayerCamera::FollowCamera(float dt) {
	//----------------------------------------
	// ① ピボット
	const Vector3 pivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;

	//----------------------------------------
	// ② 現在ブーム姿勢から前方ベクトル
	const Vector3 forward = Normalize(Transform(MakeForwardVector3(), boomRot_));

	//----------------------------------------
	// ③ 目標カメラ位置
	const Vector3 targetEye = pivot - forward * radius_;
	const Vector3 targetTarget = pivot;

	//----------------------------------------
	// ④ 補間
	float t = 1.0f - std::exp(-dt / kFollowLag_);

	eye_ = Lerp(eye_, targetEye, t);
	target_ = Lerp(target_, targetTarget, t);
}