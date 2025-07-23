#include "MechArmLeft.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/PlayerCamera/PlayerCamera.h"

using namespace MAGIMath;

MechArmLeft::MechArmLeft() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> leftArmModel = std::make_shared<ModelRenderer>("MechLeftArm", "MechLeftArm");

	std::shared_ptr<GameObject3D> leftArmObject = std::make_shared<GameObject3D>("MechLeftArm", Vector3(-0.4f, 0.2f, 0.0f));
	leftArmObject->AddModelRenderer(std::move(leftArmModel));
	leftArmObject->SetIsUnique(true);
	// ゲームオブジェクトマネージャに追加
	leftArm_ = MAGISYSTEM::AddGameObject3D(std::move(leftArmObject));

}

void MechArmLeft::Update(MechCore* mechCore) {
	// ロックオン対象がいるなら
	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		// 対象の胴体を取得
		if (auto targetBodyObj = target->GetMechBody()->GetGameObject().lock()) {
			// ロックオン対象の胴体のワールド座標を取得
			const Vector3 targetBodyPos = targetBodyObj->GetTransform()->GetWorldPosition();
			// 弾の発射地点のワールド座標を取得
			const Vector3 bulletFirePosition = mechCore->GetLeftHandWeapon()->GetFireWorldPosition();
			// 弾の発射地点からターゲットまでの距離
			const float fireToTarget = Length(bulletFirePosition - targetBodyPos);
			// 弾速を取得
			const float bulletSpeed = mechCore->GetLeftHandWeapon()->GetBulletSpeed();
			// 着弾までの予測時間
			const float timeToImpact = fireToTarget / bulletSpeed;

			// 着弾時の敵の移動後予測地点
			const Vector3 targetEstPos = targetBodyPos + target->GetMovementComponent()->GetCurrentVelocity() * timeToImpact;

			if (auto obj = leftArm_.lock()) {
				// 前方ベクトルを計算、正規化
				forward_ = targetEstPos - obj->GetTransform()->GetWorldPosition();
				forward_ = Normalize(forward_);
				// ヨー角を取得
				const float yaw = std::atan2(forward_.x, forward_.z);
				const float pitch = std::atan2(-forward_.y, std::sqrt(forward_.x * forward_.x + forward_.z * forward_.z));

				// 回転を作成
				Quaternion qYaw = MakeRotateAxisAngleQuaternion({ 0,1,0 }, yaw);
				Quaternion qPitch = MakeRotateAxisAngleQuaternion({ 1,0,0 }, pitch);
				Quaternion worldQ = Normalize(qYaw * qPitch);

				// 胴体の回転の逆行列をかける
				const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
				const Quaternion targetQ = Inverse(bodyQ) * worldQ;

				obj->GetTransform()->SetQuaternion(targetQ);
			}

		}
	} else {
		if (auto obj = leftArm_.lock()) {
			if (auto mechCoreObj = mechCore->GetGameObject().lock()) {
				if (auto camera = dynamic_cast<PlayerCamera*>(mechCoreObj->GetCamera3D("MainCamera").lock().get())) {
					const Quaternion localQ = camera->GetCameraQuaternion();
					const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
					const Quaternion targetQ = Inverse(bodyQ) * localQ;

					forward_ = Normalize(Transform(MakeForwardVector3(), localQ));

					obj->GetTransform()->SetQuaternion(targetQ);

				}
			}

		}
	}
}

std::weak_ptr<GameObject3D> MechArmLeft::GetGameObject()const {
	return leftArm_;
}

const Vector3& MechArmLeft::GetForward() const {
	return forward_;
}
