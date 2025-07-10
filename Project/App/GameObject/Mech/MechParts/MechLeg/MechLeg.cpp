#include "MechLeg.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

MechLeg::MechLeg() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> legModel = std::make_shared<ModelRenderer>("MechLeg", "MechLeg");

	std::shared_ptr<GameObject3D> legObject = std::make_shared<GameObject3D>("MechLeg", Vector3(0.0f, -0.6f, 0.0f));
	legObject->AddModelRenderer(std::move(legModel));
	legObject->SetIsUnique(true);
	// ゲームオブジェクトマネージャに追加
	leg_ = MAGISYSTEM::AddGameObject3D(std::move(legObject));
}

void MechLeg::Update(MechCore* core) {
	// 移動方向から回転を計算
	RotateAngleAsVelocity(core);
}

std::weak_ptr<GameObject3D> MechLeg::GetGameObject()const {
	return leg_;
}

void MechLeg::RotateAngleAsVelocity(MechCore* core) {
	// 現在の進行方向を取得
	const Vector2 currentDir = core->GetMovementComponent()->GetCurrentMoveDir();
	float yaw = std::atan2(currentDir.x, currentDir.y);
	const Quaternion dirQ = MakeRotateAxisAngleQuaternion({ 0.0f,1.0f,0.0f }, yaw);
	const Quaternion bodyQ = core->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
	const Quaternion targetQ = Inverse(bodyQ) * dirQ;

	if (auto leg = leg_.lock()) {
		leg->GetTransform()->SetQuaternion(targetQ);
	}
}
