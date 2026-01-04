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
	if (mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		UpdateAimToTarget(mechCore);
	} else {
		UpdateAimToCamera(mechCore);
	}
}

void BaseMechArm::UpdateAimToTarget(MechCore* mechCore) {
	auto armObj = arm_.lock();
	if (!armObj) { return; }

	auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock();
	if (!target) { return; }

	auto targetBodyObj = target->GetMechBody()->GetGameObject().lock();
	if (!targetBodyObj) { return; }

	const float bulletSpeed = GetBulletSpeed(mechCore);
	if (bulletSpeed <= 0.0001f) { return; }

	const Vector3 targetBodyPos = targetBodyObj->GetTransform()->GetWorldPosition();
	const Vector3 bulletFirePos = GetFireOffsetWorldPos(mechCore);

	const float fireToTarget = Length(bulletFirePos - targetBodyPos);
	const float timeToImpact = fireToTarget / bulletSpeed;

	const Vector3 targetEstPos =
		targetBodyPos + target->GetMovementComponent()->GetCurrentVelocity() * timeToImpact;

	const float fcsFactor = mechCore->GetStatusComponent()->GetFcsAvoidFactor();
	const Vector3 armTargetPos = Lerp(targetBodyPos, targetEstPos, fcsFactor);

	forward_ = Normalize(armTargetPos - armObj->GetTransform()->GetWorldPosition());

	const float yaw = std::atan2(forward_.x, forward_.z);
	const float pitch = std::atan2(-forward_.y, std::sqrt(forward_.x * forward_.x + forward_.z * forward_.z));

	const Quaternion qYaw = MakeRotateAxisAngleQuaternion({ 0,1,0 }, yaw);
	const Quaternion qPitch = MakeRotateAxisAngleQuaternion({ 1,0,0 }, pitch);
	const Quaternion worldQ = Normalize(qYaw * qPitch);

	const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
	const Quaternion targetQ = Inverse(bodyQ) * worldQ;

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