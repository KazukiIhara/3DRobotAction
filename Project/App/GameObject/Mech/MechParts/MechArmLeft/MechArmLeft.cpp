#include "MechArmLeft.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

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
			if (auto obj = leftArm_.lock()) {
				// 前方ベクトルを計算、正規化
				Vector3 fwd = targetBodyPos - obj->GetTransform()->GetWorldPosition();
				fwd = Normalize(fwd);
				// ヨー角を取得
				const float yaw = std::atan2(fwd.x, fwd.z);
				const float pitch = std::atan2(-fwd.y, std::sqrt(fwd.x * fwd.x + fwd.z * fwd.z));

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
		if (auto obj = leftArm_.lock()) {
			const float pitch = std::numbers::pi_v<float>*0.2f;
			Quaternion targetQ = MakeRotateAxisAngleQuaternion({ 1,0,0 }, pitch);
			obj->GetTransform()->SetQuaternion(targetQ);
		}
	}
}

std::weak_ptr<GameObject3D> MechArmLeft::GetGameObject()const {
	return leftArm_;
}