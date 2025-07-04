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

	// 入力を判定
	ApplyInput(dt);

	// ピボット計算
	pivot_ = followTargetTransform_->GetWorldPosition() + pivotOffset_;

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

	// 補間
	float t = 1.0f - std::exp(-dt / kCameraLag_);
	eye_ = Lerp(eye_, targetEye_, t);
	target_ = Lerp(target_, targetTarget_, t);

	// カメラデータ更新
	UpdateData();
}

void PlayerCamera::SetTargetTransform(Transform3D* target) {
	followTargetTransform_ = target;
}

void PlayerCamera::SetMechCore(std::weak_ptr<MechCore> mechCore) {
	core_ = mechCore;
}

Vector3 PlayerCamera::GetTargetEye()const {
	return targetEye_;
}

void PlayerCamera::ApplyInput(float dt) {
	// 右スティック入力
	Vector2 rs{};
	if (MAGISYSTEM::IsPadConnected(0)) {
		rs = MAGISYSTEM::GetRightStick(0);
	}
	if (LengthSquared(rs) < 1e-6f) return;

	// 入力を角速度へ
	pYaw_ += rs.x * sensYaw_ * dt;				// Yaw : +右
	pPitch_ -= rs.y * sensPitch_ * dt;          // Pitch: +上（符号反転）

	// 角度域の正規化
	pYaw_ = WrapPi(pYaw_);
	pPitch_ = std::clamp(pPitch_, -kPitchLim_, kPitchLim_);

	// クォータニオン合成
	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), pYaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pPitch_);

	cameraRotation_ = Normalize(qYaw * qPitch);
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

	Vector3 toTarget = targetWorldPos - pivot_;
	if (!Length(toTarget)) {      // 距離ゼロ対策
		toTarget = MakeForwardVector3();
	}
	Vector3 dir = Normalize(toTarget);   // 単位ベクトル
	float lenXZ = std::sqrt(dir.x * dir.x + dir.z * dir.z);

	pYaw_ = std::atan2(dir.x, dir.z);
	pPitch_ = std::atan2(-dir.y, lenXZ);

	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), pYaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pPitch_);
	cameraRotation_ = Normalize(qYaw * qPitch);

	forward_ = Normalize(Transform(MakeForwardVector3(), cameraRotation_));

	// 目標カメラ位置
	targetEye_ = pivot_ - forward_ * radius_;
	targetTarget_ = targetWorldPos;
}

void PlayerCamera::FollowCamera(float dt) {

	// 累積YawPitchからクオータニオンを生成
	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), pYaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pPitch_);
	cameraRotation_ = Normalize(qYaw * qPitch);

	forward_ = Normalize(Transform(MakeForwardVector3(), cameraRotation_));

	// 目標カメラ位置
	targetEye_ = pivot_ - forward_ * radius_;
	targetTarget_ = pivot_;

}