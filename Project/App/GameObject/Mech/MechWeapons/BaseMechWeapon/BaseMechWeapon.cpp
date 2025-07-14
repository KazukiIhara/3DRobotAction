#include "BaseMechWeapon.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

BaseMechWeapon::BaseMechWeapon(const std::string& weaponName, const WeaponType& type) {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> weaponModel = std::make_shared<ModelRenderer>(weaponName, weaponName);
	std::shared_ptr<GameObject3D> weaponObject = std::make_shared<GameObject3D>(weaponName);
	weaponObject->AddModelRenderer(std::move(weaponModel));
	weaponObject->SetIsUnique(true);

	// ゲームオブジェクトマネージャに追加
	weapon_ = MAGISYSTEM::AddGameObject3D(std::move(weaponObject));

	// タイプを設定
	type_ = type;

}

std::weak_ptr<GameObject3D> BaseMechWeapon::GetGameObject()const {
	return weapon_;
}

const WeaponType& BaseMechWeapon::GetType() const {
	return type_;
}

Vector3 BaseMechWeapon::GetFireWorldPosition() {
	return Vector3();
}