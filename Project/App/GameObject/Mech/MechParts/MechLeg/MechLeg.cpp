#include "MechLeg.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

using namespace MAGIMath;

MechLeg::MechLeg() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> legModel = std::make_shared<ModelRenderer>("MechLeg", "MechLeg");
	legModel->GetTransform()->SetTranslate(Vector3(0.0f, 0.5f, 0.0f));

	std::shared_ptr<GameObject3D> legObject = std::make_shared<GameObject3D>("MechLeg", Vector3(0.0f, 0.0f, 0.0f));
	legObject->AddModelRenderer(std::move(legModel));

	// ゲームオブジェクトマネージャに追加
	leg_ = MAGISYSTEM::AddGameObject3D(std::move(legObject));

}

void MechLeg::Update() {

}

std::weak_ptr<GameObject3D> MechLeg::GetGameObject()const {
	return leg_;
}