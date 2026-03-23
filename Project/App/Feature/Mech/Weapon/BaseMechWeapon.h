#pragma once

#include <string>

#include "Math/Types/AllMathTypes.h"
#include "Feature/Damage/Damage.h"

// 前方宣言
class BaseMech;

namespace Magi {
	class Transform3D;
}

// 実装メモ
/*
	パーツと違い武器によって更新処理がかなり変わると思うので
	最低限のトランスフォームと発砲場所のトランスフォームのみ実装し
	各武器はこれを継承する形で実装する
	攻撃やエフェクトは機体クラスの持つマネージャのポインタを持ってきて発生させる
*/

/// <summary>
/// 機体の武器基底クラス
/// </summary>
class BaseMechWeapon {
public:
	BaseMechWeapon(BaseMech* mech);
	virtual ~BaseMechWeapon() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DebugDraw();

	virtual void Attack(Damage::Power power = Damage::Power::Small) = 0;

	Magi::Transform3D* GetTransform();
	const Vector3 GetFireWorldPos()const;

protected:
	Magi::Transform3D* transform_ = nullptr;
	Magi::Transform3D* fireTransform_ = nullptr;

	BaseMech* mech_ = nullptr;
};