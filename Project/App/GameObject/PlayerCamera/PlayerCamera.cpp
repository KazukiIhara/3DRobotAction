#include "PlayerCamera.h"

#include "MAGI.h"

#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

PlayerCamera::PlayerCamera(const std::string& name)
	:Camera3D(name, false) {

}

void PlayerCamera::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	const float kMaxLimit = DegreeToRadian(maxPitchDegrees_);
	const float kMinLimit = DegreeToRadian(minPitchDegrees_);

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
	smoothedPivot_ = followTargetTransform_->GetWorldPosition() + pivotOffset_;
}

void PlayerCamera::SetMechCore(std::weak_ptr<MechCore> mechCore) {
	core_ = mechCore;
}

void PlayerCamera::ApplyInput(float dt) {
	// スティックの入力を取得
	Vector2 rs{};
	if (MAGISYSTEM::IsPadConnected(0)) {
		rs = MAGISYSTEM::GetRightStick(0);
	}

	// 入力がなければスキップ
	if (!Length(rs)) return;

	// Δ回転量を計算
	const float dYaw = rs.x * sensYaw_ * dt;
	const float dPitch = rs.y * sensPitch_ * dt;

	// 右軸を取得 (boomRot_ の +X 方向)
	Vector3 rightAxis = Right(boomRot_);

	// Δ回転クォータニオンを生成
	Quaternion qPitch = MakeRotateAxisAngleQuaternion(rightAxis, dPitch);
	Quaternion qYaw = MakeRotateAxisAngleQuaternion(MakeUpVector3(), dYaw);

	// 合成
	boomRot_ = Normalize(qYaw * qPitch * boomRot_);
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

	// ピボット計算
	const Vector3 pivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;

	// ピボットからターゲットの方向を計算
	Vector3 toTarget = targetWorldPos - pivot;
	if (!Length(toTarget)) {						// 距離ゼロ対策
		toTarget = MakeForwardVector3();            // (0,0,1)
	}
	toTarget = Normalize(toTarget);

	// ブーム姿勢をターゲット方向に合わせる
	Quaternion targetRot = DirectionToQuaternion(toTarget);

	boomRot_ = targetRot;

	// ブーム姿勢から軸ベクトルを取得
	const Vector3 forward = Normalize(Transform(MakeForwardVector3(), boomRot_));
	const Vector3 right = Normalize(Transform(MakeRightVector3(), boomRot_));

	// EyeとTargetを計算
	eye_ = pivot + forward * radius_;
	target_ = targetWorldPos;
}

void PlayerCamera::FollowCamera(float dt) {
	// Pivotを取得
	const Vector3 pivot = followTargetTransform_->GetWorldPosition() + pivotOffset_;

	// ブーム姿勢から各軸ベクトルを取得
	const Vector3 forward = Normalize(Transform(MakeForwardVector3(), boomRot_)); // +Z
	const Vector3 right = Normalize(Transform(MakeRightVector3(), boomRot_));   // +X

	// 位置決定
	eye_ = pivot + forward * radius_;
	target_ = pivot;                             // 正面を常に Pivot へ
}
