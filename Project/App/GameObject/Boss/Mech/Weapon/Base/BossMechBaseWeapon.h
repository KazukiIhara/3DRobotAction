#pragma once

#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;
class BossMech;

// 実装メモ
/*
	パーツと違い武器によって更新処理がかなり変わると思うので
	最低限のトランスフォームと発砲場所のトランスフォームのみ実装し
	各武器はこれを継承する形で実装する
	攻撃やエフェクトはボス機体クラスの持つマネージャのポインタを持ってきて発生させる
*/

/// <summary>
/// ボスの武器基底クラス
/// </summary>
class BossMechBaseWeapon {
public:
	BossMechBaseWeapon(BossMech* mech);
	~BossMechBaseWeapon() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void Attack() = 0;

	Transform3D* GetTransform();
	const Vector3 GetFireWorldPos()const;

protected:
	Transform3D* transform_ = nullptr;
	Transform3D* fireTransform_ = nullptr;

	BossMech* mech_ = nullptr;
};