#include "BaseMechShoulderWeapon.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

BaseMechShoulderWeapon::BaseMechShoulderWeapon(const std::string& weaponName, const ShoulderWeaponType& type, const WitchShoulder& witch) {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> weaponModel = std::make_shared<ModelRenderer>(weaponName, weaponName);
	std::shared_ptr<GameObject3D> weaponObject = std::make_shared<GameObject3D>(weaponName);
	weaponObject->AddModelRenderer(std::move(weaponModel));
	weaponObject->SetIsUnique(true);

	// ゲームオブジェクトマネージャに追加
	weapon_ = MAGISYSTEM::AddGameObject3D(std::move(weaponObject));

	// タイプを設定
	type_ = type;

	// 左右どちらかを設定
	witchShoulder_ = witch;
}

std::weak_ptr<GameObject3D> BaseMechShoulderWeapon::GetGameObject()const {
	return weapon_;
}

const ShoulderWeaponType& BaseMechShoulderWeapon::GetType() const {
	return type_;
}

Vector3 BaseMechShoulderWeapon::GetFireWorldPosition() {
	return Vector3();
}

const int32_t& BaseMechShoulderWeapon::GetDamage()const {
	return damage_;
}

bool BaseMechShoulderWeapon::IsCoolTime() const {
	return isCoolTime_;
}