#pragma once

// C++
#include <memory>

// MyHeader
#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;
class AttackCollider;

// 実装メモ
/*
	コライダーの衝突フラグを参照して自身とコライダーに削除フラグを立て、次のマネージャの処理で両方消す
	弾を消したい場合は、コライダーの持つ衝突フラグを立てれば良い
*/

/// <summary>
/// 攻撃オブジェクトの共通基底
/// </summary>
class BaseAttackObject {
public:
	BaseAttackObject();
	virtual ~BaseAttackObject();

	bool GetIsAlive() const;
	AttackCollider* GetAttackCollider();

	Vector3 GetWorldPos();

protected:
	void BeginLife(float lifeTime, std::weak_ptr<AttackCollider> collider);

	bool CheckHitAndFinalize();
	bool TickLifeAndFinalize(float dt);

	std::shared_ptr<AttackCollider> LockCollider() const;

	void Finalize();

protected:
	Transform3D* transform_ = nullptr;
	bool isAlive_ = false;

private:
	float lifeTime_ = 0.0f;
	std::weak_ptr<AttackCollider> collider_;
};
