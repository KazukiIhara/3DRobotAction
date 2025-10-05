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
enum HandWeaponType {
	AssultRifle,
};

/// <summary>
/// 手持ち武器ベースクラス
/// </summary>
class BaseMechHandWeapon {
public:
	BaseMechHandWeapon(const std::string& weaponName, const HandWeaponType& type);
	virtual ~BaseMechHandWeapon() = default;

	virtual void Update(MechCore* mechCore) = 0;

	std::weak_ptr<GameObject3D> GetGameObject()const;

	// 武器の種類を取得
	const HandWeaponType& GetType()const;
	// ダメージを取得
	const int32_t& GetDamage()const;
	// 弾速を取得
	const float GetBulletSpeed()const;

	// 攻撃を発射するワールド座標の取得
	virtual Vector3 GetFireWorldPosition();

	virtual void SetReloadTime() {};
	virtual void SetCoolTime() {};

	bool IsReloading()const;
	bool IsCoolTime()const;

protected:
	// 武器タイプ
	HandWeaponType type_;

	// ダメージ
	int32_t damage_ = 0;
	// 弾速
	float bulletSpeed_ = 0.0f;

	// オブジェクト
	std::weak_ptr<GameObject3D> weapon_;

	// リロード中
	bool isReloading_ = false;
	float reloadingTimer_ = 0.0f;

	// クールタイム
	bool isCoolTime_ = false;
	float coolTimer_ = 0.0f;
};