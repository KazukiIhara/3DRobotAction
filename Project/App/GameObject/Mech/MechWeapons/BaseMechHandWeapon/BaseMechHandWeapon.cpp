#include "BaseMechHandWeapon.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

BaseMechHandWeapon::BaseMechHandWeapon(const std::string& weaponName, const HandWeaponType& type) {
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

std::weak_ptr<GameObject3D> BaseMechHandWeapon::GetGameObject()const {
	return weapon_;
}

const HandWeaponType& BaseMechHandWeapon::GetType() const {
	return type_;
}

const int32_t& BaseMechHandWeapon::GetDamage()const {
	return damage_;
}

const float BaseMechHandWeapon::GetBulletSpeed() const {
	return bulletSpeed_;
}

Vector3 BaseMechHandWeapon::GetFireWorldPosition() {
	return Vector3();
}

bool BaseMechHandWeapon::IsReloading() const {
	return isReloading_;
}

bool BaseMechHandWeapon::IsCoolTime() const {
	return isCoolTime_;
}
