#include "MechBody.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

using namespace MAGIMath;

MechBody::MechBody() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> bodyModel = std::make_shared<ModelRenderer>("MechBody", "MechBody");
	bodyModel->GetTransform()->SetTranslate(Vector3(0.0f, 1.1f, 0.0f));

	std::shared_ptr<GameObject3D> bodyObject = std::make_shared<GameObject3D>("MechBody");
	bodyObject->AddModelRenderer(std::move(bodyModel));

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
	if (auto coreObj = mechCore->GetGameObject().lock()) {

	}
}