#pragma once

// C++
#include <memory>
#include <string>

#include "Math/Types/AllMathTypes.h"

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

	virtual Vector3 GetFireWorldPosition();

	virtual void SetReloadTime() {};
	virtual void SetCoolTime() {};

	bool IsReloading()const;
	bool IsCoolTime()const;

protected:
	// 武器タイプ
	WeaponType type_;

	// オブジェクト
	std::weak_ptr<GameObject3D> weapon_;


	// リロード中
	bool isReloading_ = false;
	float reloadingTimer_ = 0.0f;

	// クールタイム
	bool isCoolTime_ = false;
	float coolTimer_ = 0.0f;
};