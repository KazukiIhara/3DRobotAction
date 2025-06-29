#include "MechArmRight.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

using namespace MAGIMath;

MechArmRight::MechArmRight() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> rightArmModel = std::make_shared<ModelRenderer>("MechRightArm", "MechRightArm");

	std::shared_ptr<GameObject3D> rightArmObject = std::make_shared<GameObject3D>("MechRightArm", Vector3(0.5f, 0.0f, 0.0f));
	rightArmObject->AddModelRenderer(std::move(rightArmModel));

	// ゲームオブジェクトマネージャに追加
	rightArm_ = MAGISYSTEM::AddGameObject3D(std::move(rightArmObject));

}

void MechArmRight::Update(MechCore* mechCore) {

}

std::weak_ptr<GameObject3D> MechArmRight::GetGameObject()const {
	return rightArm_;
}