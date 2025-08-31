#include "PlayerCamera.h"

#include "MAGI.h"

#include "GameObject/Mech/MechCore/MechCore.h"


using namespace MAGIMath;

PlayerCamera::PlayerCamera(const std::string& name, float yaw)
	:Camera3D(name, false) {
	pYaw_ = yaw;
}

void PlayerCamera::Update() {
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

	if (auto core = core_.lock()) {
		// ハードロックオンオフフラグ取得
		if (core->GetLockOnComponent()->GetEnableHardLockOn()) {
			if (core->GetLockOnComponent()->GetLockOnTarget().lock()) { // ターゲットあり
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

void PlayerCamera::SetTargetTransform(Transform3D* target) {
	followTargetTransform_ = target;
}

void PlayerCamera::SetMechCore(std::weak_ptr<MechCore> mechCore) {
	core_ = mechCore;
	eye_ = core_.lock()->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();
}

const Quaternion& PlayerCamera::GetCameraQuaternion() const {
	return cameraRotation_;
}

void PlayerCamera::ApplyInput(float dt) {

	// 右スティック入力
	Vector2 rs{};
	if (auto core = core_.lock()) {
		rs = core->GetInputCommand().cameraRotDirection;
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

void PlayerCamera::HardLockCamera(float dt) {

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

void PlayerCamera::FollowCamera() {
	// 累積YawPitchからクオータニオンを生成
	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), pYaw_);
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(MakeRightVector3(), pPitch_);
	cameraRotation_ = Normalize(qYaw * qPitch);

	forward_ = Normalize(Transform(MakeForwardVector3(), cameraRotation_));

	// ターゲット設定
	targetTarget_ = pivot_;

}