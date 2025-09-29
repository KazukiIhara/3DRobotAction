#pragma once

// C++
#include <memory>
#include <string>

// MyHedder
#include "Math/Types/AllMathTypes.h"

// 前方宣言
class MechCore;
class GameObject3D;

/// <summary>
/// 種類
/// </summary>
enum class ShoulderWeaponType {
	DualMissileLauncher,
};

enum class WitchShoulder {
	Left,
	Right,
};

/// <summary>
/// 肩武器のベース
/// </summary>
class BaseMechShoulderWeapon {
public:
	BaseMechShoulderWeapon(const std::string& weaponName, const ShoulderWeaponType& type, const WitchShoulder& witch);
	virtual ~BaseMechShoulderWeapon() = default;

	virtual void Update(MechCore* mechCore) = 0;

	std::weak_ptr<GameObject3D> GetGameObject()const;

	// 攻撃を発射するワールド座標の取得
	virtual Vector3 GetFireWorldPosition();

	// 武器の種類を取得
	const ShoulderWeaponType& GetType()const;
	// ダメージを取得
	const int32_t& GetDamage()const;

	// クールタイム中かどうか
	bool IsCoolTime()const;

protected:
	// オブジェクト
	std::weak_ptr<GameObject3D> weapon_;

	// 武器タイプ
	ShoulderWeaponType type_;

	// 左右どちらの肩か
	WitchShoulder witchShoulder_;

	// ダメージ
	int32_t damage_ = 0;

	// クールタイム
	bool isCoolTime_ = false;
	float coolTimer_ = 0.0f;
};