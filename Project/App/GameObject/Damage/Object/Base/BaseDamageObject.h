#pragma once


#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;
class DamageCollider;
class DamageCollisionSystem;

// 実装メモ
/*
	派生先のコンストラクタ内で、コライダーマネージャを使って任意のコライダーを作成する
	コライダー内の更新処理でなく、このクラスの派生がコライダーの動きの責任も持つ
	既定のFinalizeは派生先でも必ず呼ぶ
*/

/// <summary>
/// ダメージオブジェクトの基底クラス
/// </summary>
class BaseDamageObject {
public:
	BaseDamageObject(const Vector3& worldPos);
	virtual ~BaseDamageObject() = default;

	void SetCollisionSystem(DamageCollisionSystem* collisionSystem);
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize();

	DamageCollider* GetDamageCollider();
	bool GetIsAlive()const;

protected:
	// トランスフォーム
	Transform3D* transform_ = nullptr;
	// 生存フラグ
	bool isAlive_ = true;

private:
	// コライダーの参照ポインタ
	DamageCollider* damageCollider_ = nullptr;
	// コライダーマネージャの参照ポインタ
	DamageCollisionSystem* collisionSystem_ = nullptr;
};