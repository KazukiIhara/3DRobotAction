#include "PlayerCamera.h"

#include "MAGI.h"

#include "GameObject/Mech/MechCore/MechCore.h"


using namespace magi;
using namespace MAGIMath;

MechCamera::MechCamera(const std::string& name, float yaw, MechCore* core)
	:Camera3D(name, false) {
	pYaw_ = yaw;
	core_ = core;

	if (auto body = core_->GetMechBody()->GetGameObject().lock()) {
		eye_ = body->GetTransform()->GetWorldPosition();
		target_ = eye_;
		target_.z += 1.0f;
	}
}

void MechCamera::Update() {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ターゲットがいなければ早期リターン
	if (!followTargetTransform_) return;

	// 入力を判定
	ApplyInput(dt);

	// 補間
	const float pivotT = 1.0f - std::exp(-dt / kPivotLag_);

	// ピボット計算
	targetPivot_ = followTargetTransform_->GetWorldPosition() + pivotOffset_;
	pivot_ = Lerp(pivot_, targetPivot_, pivotT);

	if (core_) {
		// ハードロックオンオフフラグ取得
		if (core_->GetLockOnComponent()->GetEnableHardLockOn()) {
			if (core_->GetLockOnComponent()->GetLockOnTarget().lock()) { // ターゲットあり
				HardLockCamera(dt);
			} else { // ターゲットなし
				FollowCamera();
			}
		} else { // ハードロックオンオフ
			FollowCamera();
		}
	}

	// 前方ベクトルと半径からカメラの目標座標を計算
	targetEye_ = pivot_ - forward_ * radius_;

	// ロックオン関数で計算した値を取得
	eye_ = targetEye_;
	target_ = targetTarget_;

	// カメラデータ更新
	UpdateData();
}

void MechCamera::SetTargetTransform(Transform3D* target) {
	followTargetTransform_ = target;
}

void MechCamera::SetCameraQuaternion(const Quaternion& q) {
	cameraRotation_ = q;
}

const Quaternion& MechCamera::GetCameraQuaternion() const {
	return cameraRotation_;
}

void MechCamera::ApplyInput(float dt) {
	// 右スティック入力
	Vector2 rs{};
	if (core_) {
		// 自機でない場合早期リターン
		if (core_->GetFriendlyTag() == FriendlyTag::EnemySide) {
			return;
		}

		rs = core_->GetInputCommand().cameraRotDirection;
	}

	// 入力がない、小さすぎる場合は早期リターン
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

void MechCamera::HardLockCamera(float dt) {

	// ターゲット座標取得
	Vector3 targetWorldPos{};
	if (core_) {
		if (auto tgt = core_->GetLockOnComponent()->GetLockOnTarget().lock()) {
			if (auto obj = tgt->GetMechBody()->GetGameObject().lock()) {
				targetWorldPos = obj->GetTransform()->GetWorldPosition();
			}
		}
	}

	Vector3 toTarget = targetWorldPos - pivot_;
	if (!Length(toTarget)) {
		toTarget = MakeForwardVector3();
	}
	Vector3 dir = Normalize(toTarget);
	float lenXZ = std::sqrt(dir.x * dir.x + dir.z * dir.z);

	const float rotT = 1.0f - std::exp(-dt / kHardLockRotLag_);
	const float targetYaw = std::atan2(dir.x, dir.z);
	const float targetPitch = std::atan2(-dir.y, lenXZ);

	pYaw_ = LerpAngle(pYaw_, targetYaw, rotT);
	pPitch_ = LerpAngle(pPitch_, targetPitch, rotT);

	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), pYaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pPitch_);
	cameraRotation_ = Normalize(qYaw * qPitch);

	forward_ = Normalize(Transform(MakeForwardVector3(), cameraRotation_));

	// ターゲット設定
	const float targetT = 1.0f - std::exp(-dt / kHardLockTargetLag_);

	targetTarget_ = Lerp(targetTarget_, targetWorldPos, targetT);

}

void MechCamera::FollowCamera() {

	// 前方を計算
	forward_ = Normalize(Transform(MakeForwardVector3(), cameraRotation_));

	// ターゲット設定
	targetTarget_ = pivot_;

}
