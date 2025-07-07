#pragma once

// C++
#include <memory>
#include <string>

// 前方宣言
class MechCore;
class GameObject3D;

/// <summary>
/// 武器ベースクラス
/// </summary>
class BaseMechWeapon {
public:
	BaseMechWeapon(const std::string& weaponName);
	virtual ~BaseMechWeapon() = default;
	virtual void Update(MechCore* mechCore) = 0;
	std::weak_ptr<GameObject3D> GetGameObject()const;
private:
	// オブジェクト
	std::weak_ptr<GameObject3D> weapon_;
};