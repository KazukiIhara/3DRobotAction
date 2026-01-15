#pragma once

#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;
class AttackObjectManager;
class GameEffectManager;

/// <summary>
/// ボスの武器基底クラス
/// </summary>
class BossMechBaseWeapon {
public:
	BossMechBaseWeapon();
	~BossMechBaseWeapon() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	virtual void Attack() = 0;

	Transform3D* GetTransform();
	const Vector3 GetFireWorldPos()const;

protected:
	Transform3D* transform_ = nullptr;
	Transform3D* fireTransform_ = nullptr;
};