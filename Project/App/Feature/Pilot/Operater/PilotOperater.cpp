#include "PilotOperater.h"

#include "MAGI.h"

#include "Feature/TPSCamera3D/TPSCamera3D.h"

using namespace Magi;

PilotOperator::PilotOperator(PilotMech* mech, TPSCamera3D* camera) {
	mech_ = mech;
	camera_ = camera;
}

void PilotOperator::Update() {
	// メニュー操作

	// カメラ操作
	CameraOperation();

	// 機体操作

}

void PilotOperator::CameraOperation() {
	if (!camera_) {
		return;
	}

	const int kPadId = 0;
	if (!MAGISYSTEM::IsPadConnected(kPadId)) {
		return;
	}

	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 右スティック入力
	Vector2 rs = MAGISYSTEM::GetRightStick(kPadId);

	// 感度
	const float yawSens = 2.2f;
	const float pitchSens = 2.0f;

	// yaw/pitch 反映
	camera_->AddYaw(rs.x * yawSens * dt);
	camera_->AddPitch(rs.y * pitchSens * dt);
}
