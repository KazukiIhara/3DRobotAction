#include "MechBody.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

using namespace MAGIMath;

MechBody::MechBody() {
	// レンダラーとゲームオブジェクトを作成 TODO: 引数からオブジェクトを作るようにする
	std::shared_ptr<ModelRenderer> coreModel = std::make_shared<ModelRenderer>("MechCore", "MechCore");
	coreModel->GetTransform()->SetTranslate(Vector3(0.0f, 1.0f, 0.0f));

	std::shared_ptr<GameObject3D> coreObject = std::make_shared<GameObject3D>("MechCore", Vector3(0.0f, 50.0f, 0.0f));
	coreObject->AddModelRenderer(std::move(coreModel));


}

void MechBody::Update() {

}