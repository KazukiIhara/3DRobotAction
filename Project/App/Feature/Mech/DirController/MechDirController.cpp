#include "MechDirController.h"

#include "Math/Utility/MathUtility.h"

#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/Base/BaseMech.h"

using namespace MAGIMath;

MechDirController::MechDirController(BaseMech* mech) {
	mech_ = mech;


}

void MechDirController::Update() {
	// 機体を回転
	RotateMech();

	// 次のフレーム用にフラグをリセット
	lookAtFlag_ = {};
}

MechDirController::LookAtFlag& MechDirController::GetLookAtFlag() {
	return lookAtFlag_;
}

void MechDirController::RotateMech() {
	// 両方Falseなら早期リターン
	if (!lookAtFlag_.mech.yaw && !lookAtFlag_.mech.pitch) {
		return;
	}
	// 機体の中心
	const Vector3 mechCenter = mech_->GetCenterPos();
	// ターゲットの中心
	const Vector3 targetCenter = mech_->GetTargetWorldPos();
	// 向きを計算
	const Vector3 dir = targetCenter - mechCenter;

	// 機体
	if (lookAtFlag_.mech.yaw && lookAtFlag_.mech.pitch) {
		// クオータニオンを計算
		const Quaternion targetQ = DirectionToQuaternion_s(dir);
		// ひとまず値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	} else if (lookAtFlag_.mech.yaw) {
		// クオータニオンを計算
		const Quaternion targetQ = DirectionToQuaternionYaw_s(dir);
		// ひとまず値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	} else if (lookAtFlag_.mech.pitch) {
		// クオータニオンを計算
		const Quaternion targetQ = DirectionToQuaternionPitch_s(dir);
		// ひとまず値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	}

}

void MechDirController::RotateHead() {

}

void MechDirController::RotateBody() {

}
