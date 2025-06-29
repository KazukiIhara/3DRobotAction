#include "MechArmLeft.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

using namespace MAGIMath;

MechArmLeft::MechArmLeft() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> leftArmModel = std::make_shared<ModelRenderer>("MechLeftArm", "MechLeftArm");

	std::shared_ptr<GameObject3D> leftArmObject = std::make_shared<GameObject3D>("MechLeftArm", Vector3(0.4f, 0.2f, 0.0f));
	leftArmObject->AddModelRenderer(std::move(leftArmModel));

	// ゲームオブジェクトマネージャに追加
	leftArm_ = MAGISYSTEM::AddGameObject3D(std::move(leftArmObject));

}

void MechArmLeft::Update(MechCore* mechCore) {

}

std::weak_ptr<GameObject3D> MechArmLeft::GetGameObject()const {
	return leftArm_;
}