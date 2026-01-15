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

	void Update();
	void Draw();

	virtual void Attack() = 0;

	Transform3D* GetTransfom();

private:
	Transform3D* transform_;
	Vector3 attackWorldPos_;

	AttackObjectManager* attackObjectManager_;
	GameEffectManager* gameEffectManager_;
};