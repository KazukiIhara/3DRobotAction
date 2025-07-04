#include "BaseMechWeapon.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

BaseMechWeapon::BaseMechWeapon(const std::string& weaponName) {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> weaponModel = std::make_shared<ModelRenderer>(weaponName, weaponName);
	std::shared_ptr<GameObject3D> weaponObject = std::make_shared<GameObject3D>(weaponName, Vector3(0.0f, 1.0f, 0.0f));
	weaponObject->AddModelRenderer(std::move(weaponModel));

	// ゲームオブジェクトマネージャに追加
	weapon_ = MAGISYSTEM::AddGameObject3D(std::move(weaponObject));
}
