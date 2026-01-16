#pragma once


#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;
class BaseDamageCollider;
class DamageCollisonManager;

/// <summary>
/// ダメージオブジェクトの基底クラス
/// </summary>
class BaseDamageObject {
public:
	BaseDamageObject(const Vector3& worldPos);
	~BaseDamageObject() = default;

	void SetCollisonManager(DamageCollisonManager* collisionManager);
	virtual void Update() = 0;
	virtual void Draw() = 0;

	BaseDamageCollider* GetDamageCollider();
	bool GetIsAlive()const;

protected:
	// トランスフォーム
	Transform3D* transform_ = nullptr;
	// 生存フラグ
	bool isAlive_ = false;

private:
	// コライダーの参照ポインタ
	BaseDamageCollider* damageCollider_ = nullptr;
	// コライダーマネージャの参照ポインタ
	DamageCollisonManager* collisionManager_ = nullptr;
};