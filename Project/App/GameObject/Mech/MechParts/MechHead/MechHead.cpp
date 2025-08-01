#include "MechHead.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

MechHead::MechHead() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> headModel = std::make_shared<ModelRenderer>("MechHead", "MechHead");

	std::shared_ptr<GameObject3D> headObject = std::make_shared<GameObject3D>("MechHead", Vector3(0.0f, 0.5f, 0.0f));
	headObject->SetIsUnique(true);
	headObject->AddModelRenderer(std::move(headModel));

	// ゲームオブジェクトマネージャに追加
	head_ = MAGISYSTEM::AddGameObject3D(std::move(headObject));
}

void MechHead::Update([[maybe_unused]] MechCore* mechCore) {

}

std::weak_ptr<GameObject3D> MechHead::GetGameObject()const {
	return head_;
}