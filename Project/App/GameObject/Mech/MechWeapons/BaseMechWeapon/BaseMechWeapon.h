#pragma once

// C++
#include <memory>
#include <string>

// 前方宣言
class MechCore;
class GameObject3D;

/// <summary>
/// 武器の種類
/// </summary>
enum WeaponType {
	AssultRifle,
};

/// <summary>
/// 武器ベースクラス
/// </summary>
class BaseMechWeapon {
public:
	BaseMechWeapon(const std::string& weaponName, const WeaponType& type);
	virtual ~BaseMechWeapon() = default;
	virtual void Update(MechCore* mechCore) = 0;
	std::weak_ptr<GameObject3D> GetGameObject()const;
	const WeaponType& GetType()const;

protected:
	// 武器タイプ
	WeaponType type_;

	// オブジェクト
	std::weak_ptr<GameObject3D> weapon_;
};