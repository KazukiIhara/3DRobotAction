#include "MechRotControlSystem.h"

#include "Math/Utility/MathUtility.h"

#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/Base/BaseMech.h"

#include "MAGI.h"

using namespace Magi;
using namespace MAGIMath;

MechRotControlSystem::MechRotControlSystem(BaseMech* mech) {
	mech_ = mech;
	lookAtFlag_ = {};
	turnToMoveDir_ = false;
}

void MechRotControlSystem::Update() {
	// 機体を移動方向に向ける
	TurnToMoveDirection();

	// フラグに応じて機体を回転
	RotateMech();

	// 次のフレーム用にフラグをリセット
	turnToMoveDir_ = false;
	lookAtFlag_ = {};
}

MechRotControlSystem::LookAtFlag& MechRotControlSystem::GetLookAtFlag() {
	return lookAtFlag_;
}

void MechRotControlSystem::SetTurnToMoveDir(bool flag) {
	turnToMoveDir_ = flag;
}

void MechRotControlSystem::TurnToMoveDirection() {
	if (lookAtFlag_.mech.yaw || lookAtFlag_.mech.pitch) {
		return;
	}
	if (turnToMoveDir_) {
		const Vector3 dir = mech_->GetMoveSystem()->GetDir();
		const float dt = MAGISYSTEM::GetDeltaTime();
		const float expT = CalExpT(dt, 0.5f, 1.0f);
		const Quaternion curQ = mech_->GetModelTransform()->GetQuaternion();
		// クオータニオンを計算
		const Quaternion targetQ = Slerp(curQ, DirectionToQuaternion_s(dir), expT);
		// 値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	}
}

void MechRotControlSystem::RotateMech() {
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

	const float dt = MAGISYSTEM::GetDeltaTime();
	const float expT = CalExpT(dt, 0.5f, 1.0f);
	const Quaternion curQ = mech_->GetModelTransform()->GetQuaternion();

	// 機体
	if (lookAtFlag_.mech.yaw && lookAtFlag_.mech.pitch) {
		// クオータニオンを計算
		const Quaternion targetQ = Slerp(curQ, DirectionToQuaternion_s(dir), expT);
		// ひとまず値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	} else if (lookAtFlag_.mech.yaw) {
		// クオータニオンを計算
		const Quaternion targetQ = Slerp(curQ, DirectionToQuaternionYaw_s(dir), expT);
		// ひとまず値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	} else if (lookAtFlag_.mech.pitch) {
		// クオータニオンを計算
		const Quaternion targetQ = Slerp(curQ, DirectionToQuaternionPitch_s(dir), expT);
		// ひとまず値をぶち込む
		mech_->GetModelTransform()->SetQuaternion(targetQ);
	}

}

void MechRotControlSystem::RotateHead() {

}

void MechRotControlSystem::RotateBody() {

}
