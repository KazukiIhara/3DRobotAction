#include "BaseMechArm.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/PlayerCamera/PlayerCamera.h"

using namespace MAGIMath;

BaseMechArm::BaseMechArm(const Param& param) {
	param_ = param;

	std::shared_ptr<ModelRenderer> model =
		std::make_shared<ModelRenderer>(param_.modelName, param_.modelName);

	std::shared_ptr<GameObject3D> obj =
		std::make_shared<GameObject3D>(param_.modelName, param_.offSetPos);

	obj->AddModelRenderer(std::move(model));
	obj->SetIsUnique(true);

	arm_ = MAGISYSTEM::AddGameObject3D(std::move(obj));

	{
		std::unique_ptr<Transform3D> shoulder = std::make_unique<Transform3D>(Vector3(0.0f, 0.0f, 0.0f));
		shoulderTransform_ = MAGISYSTEM::AddTransform3D(std::move(shoulder));
	}
	{
		std::unique_ptr<Transform3D> hand = std::make_unique<Transform3D>(Vector3(0.0f, 0.0f, 0.6f));
		handTransform_ = MAGISYSTEM::AddTransform3D(std::move(hand));
	}

	if (auto armObj = arm_.lock()) {
		shoulderTransform_->SetParent(armObj->GetTransform(), false);
		handTransform_->SetParent(shoulderTransform_, false);
	}
}

void BaseMechArm::Update(MechCore* mechCore) {
	if (mechCore->GetLockOnComponent()->GetLockOnTarget()) {
		UpdateAimToTarget(mechCore);
	} else {
		UpdateAimToCamera(mechCore);
	}
}

void BaseMechArm::UpdateAimToTarget(MechCore* mechCore) {
	// アームのゲームオブジェクトを取得
	auto armObj = arm_.lock();
	if (!armObj) { return; }

	// ロックオン中のターゲットを取得
	auto target = mechCore->GetLockOnComponent()->GetLockOnTarget();
	if (!target) { return; }

	// ターゲットのボディオブジェクトを取得
	auto targetBodyObj = target->GetMechBody()->GetGameObject().lock();
	if (!targetBodyObj) { return; }

	// 弾速を取得
	const float bulletSpeed = GetBulletSpeed(mechCore);
	if (bulletSpeed <= 0.0001f) { return; }

	// ターゲットの現在位置を取得
	const Vector3 targetBodyPos = targetBodyObj->GetTransform()->GetWorldPosition();
	// 弾の発射位置を取得
	const Vector3 bulletFirePos = GetFireOffsetWorldPos(mechCore);

	// 発射位置からターゲットまでの距離を計算
	const float fireToTarget = Length(bulletFirePos - targetBodyPos);
	// 着弾までにかかる時間を計算
	const float timeToImpact = fireToTarget / bulletSpeed;

	// ターゲットの移動速度を取得
	Vector3 targetVelocity = target->GetMovementComponent()->GetCurrentVelocity();
	// 下方向への移動は考慮しない
	targetVelocity.y = std::max(0.0f, targetVelocity.y);

	// 着弾時点のターゲット予測位置を計算
	const Vector3 targetEstPos = targetBodyPos + targetVelocity * timeToImpact;

	// FCSによる補正係数を取得
	const float fcsFactor = mechCore->GetStatusComponent()->GetFcsAvoidFactor();
	// 現在位置と予測位置を補間した最終的な狙い位置を計算
	const Vector3 armTargetPos = Lerp(targetBodyPos, targetEstPos, fcsFactor);

	// アームから狙い位置への前方ベクトルを計算
	forward_ = Normalize(armTargetPos - armObj->GetTransform()->GetWorldPosition());

	// 前方ベクトルからYaw角を算出
	const float yaw = std::atan2(forward_.x, forward_.z);
	// 前方ベクトルからPitch角を算出
	const float pitch = std::atan2(-forward_.y, std::sqrt(forward_.x * forward_.x + forward_.z * forward_.z));

	// Y軸回転用クォータニオンを生成
	const Quaternion qYaw = MakeRotateAxisAngleQuaternion({ 0,1,0 }, yaw);
	// X軸回転用クォータニオンを生成
	const Quaternion qPitch = MakeRotateAxisAngleQuaternion({ 1,0,0 }, pitch);
	// ワールド空間での最終回転を合成
	const Quaternion worldQ = Normalize(qYaw * qPitch);

	// 機体ボディの回転を取得
	const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
	// ボディローカル空間でのアーム目標回転を計算
	const Quaternion targetQ = Inverse(bodyQ) * worldQ;

	// アームの回転を更新
	armObj->GetTransform()->SetQuaternion(targetQ);
}


void BaseMechArm::UpdateAimToCamera(MechCore* mechCore) {
	auto armObj = arm_.lock();
	if (!armObj) { return; }

	auto mechCoreObj = mechCore->GetGameObject().lock();
	if (!mechCoreObj) { return; }

	auto camera = dynamic_cast<MechCamera*>(mechCoreObj->GetCamera3D("MainCamera"));
	if (!camera) { return; }

	const Quaternion cameraQ = camera->GetCameraQuaternion();
	const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
	const Quaternion targetQ = Inverse(bodyQ) * cameraQ;

	forward_ = Normalize(Transform(MakeForwardVector3(), cameraQ));
	armObj->GetTransform()->SetQuaternion(targetQ);
}

std::weak_ptr<GameObject3D> BaseMechArm::GetGameObject() const {
	return arm_;
}

Transform3D* BaseMechArm::GetShoulderTransform() {
	return shoulderTransform_;
}

Transform3D* BaseMechArm::GetHandTransform() {
	return handTransform_;
}

const Vector3& BaseMechArm::GetForward() const {
	return forward_;
}

void BaseMechArm::DebugDraw() {

}