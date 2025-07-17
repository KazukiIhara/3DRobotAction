#include "MechBody.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

MechBody::MechBody() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> bodyModel = std::make_shared<ModelRenderer>("MechBody", "MechBody");

	std::shared_ptr<GameObject3D> bodyObject = std::make_shared<GameObject3D>("MechBody", Vector3(0.0f, 1.0f, 0.0f));
	bodyObject->AddModelRenderer(std::move(bodyModel));
	bodyObject->SetIsUnique(true);
	// ゲームオブジェクトマネージャに追加
	body_ = MAGISYSTEM::AddGameObject3D(std::move(bodyObject));

}

void MechBody::Update(MechCore* mechCore) {
	// 方向を指定
	DirectionToLockOnView(mechCore);

}

std::weak_ptr<GameObject3D> MechBody::GetGameObject()const {
	return body_;
}

void MechBody::DirectionToLockOnView(MechCore* mechCore) {
	// ロックオン用のカメラ情報を取得
	const LockOnView view = mechCore->GetLockOnView();
	// カメラの前方向を計算 
	Vector3 camFwd = view.target - view.eye;
	// 平面回転に限定するため Y を無効化
	camFwd.y = 0.0f;
	// 正規化
	camFwd = Normalize(camFwd);
	// ヨー角を取得
	const float yaw = std::atan2(camFwd.x, camFwd.z);
	// 回転を作成、適用
	const Quaternion targetQ = MakeRotateAxisAngleQuaternion({ 0.0f,1.0f,0.0f }, yaw);

	if (auto body = body_.lock()) {
		body->GetTransform()->SetQuaternion(targetQ);
	}
}