#include "MechArmRight.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

MechArmRight::MechArmRight() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> rightArmModel = std::make_shared<ModelRenderer>("MechRightArm", "MechRightArm");

	std::shared_ptr<GameObject3D> rightArmObject = std::make_shared<GameObject3D>("MechRightArm", Vector3(0.4f, 0.2f, 0.0f));
	rightArmObject->AddModelRenderer(std::move(rightArmModel));
	rightArmObject->SetIsUnique(true);
	// ゲームオブジェクトマネージャに追加
	rightArm_ = MAGISYSTEM::AddGameObject3D(std::move(rightArmObject));

}

void MechArmRight::Update(MechCore* mechCore) {
	// ロックオン対象がいるなら
	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		// 対象の胴体を取得
		if (auto targetBodyObj = target->GetMechBody()->GetGameObject().lock()) {
			// ロックオン対象の胴体のワールド座標を取得
			const Vector3 targetBodyPos = targetBodyObj->GetTransform()->GetWorldPosition();
			if (auto obj = rightArm_.lock()) {
				// 前方ベクトルを計算、正規化
				forward_ = targetBodyPos - obj->GetTransform()->GetWorldPosition();
				forward_ = Normalize(forward_);
				// ヨー角を取得
				const float yaw = std::atan2(forward_.x, forward_.z);
				const float pitch = std::atan2(-forward_.y, std::sqrt(forward_.x * forward_.x + forward_.z * forward_.z));

				// 回転を作成
				Quaternion qYaw = MakeRotateAxisAngleQuaternion({ 0,1,0 }, yaw);
				Quaternion qPitch = MakeRotateAxisAngleQuaternion({ 1,0,0 }, pitch);
				Quaternion worldQ = Normalize(qPitch * qYaw);

				// 胴体の回転の逆行列をかける
				const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
				const Quaternion targetQ = worldQ * Inverse(bodyQ);
				obj->GetTransform()->SetQuaternion(targetQ);
			}

		}
	} else {
		if (auto obj = rightArm_.lock()) {
			const float pitch = 0.0f;
			const Quaternion localQ{};
			const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
			const Quaternion targetQ = localQ * bodyQ;
			forward_ = Normalize(Transform(MakeForwardVector3(), targetQ));

			// ローカルのクオータニオンをセット
			obj->GetTransform()->SetQuaternion(localQ);
		}
	}
}

std::weak_ptr<GameObject3D> MechArmRight::GetGameObject()const {
	return rightArm_;
}

const Vector3& MechArmRight::GetForward() const {
	return forward_;
}

