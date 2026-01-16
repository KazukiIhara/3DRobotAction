#pragma once


#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;
class BaseDamageCollider;

/// <summary>
/// ダメージオブジェクトの基底クラス
/// </summary>
class BaseDamageObject {
public:
	BaseDamageObject(const Vector3& worldPos, BaseDamageCollider* damageCollider);
	~BaseDamageObject() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

	BaseDamageCollider* GetDamageCollider();

protected:
	// トランスフォーム
	Transform3D* transform_ = nullptr;

private:
	BaseDamageCollider* damageCollider_;
};