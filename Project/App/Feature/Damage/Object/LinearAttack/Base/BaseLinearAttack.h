#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

/// <summary>
/// 線上の攻撃基底クラス
/// </summary>
class BaseLinearAttack:public BaseDamageObject {
public:
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float speed = 0.0f;
		float life = 0.0f;
		float damage = 0.0f;
		DamageCollider::GameParam gParam;
	};
public:
	BaseLinearAttack(
		const BaseLinearAttack::InitParam& initParam,
		DamageCollisionSystem* damageCollisionSystem
	);
	~BaseLinearAttack() = default;

	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

protected:
	Vector3 endPos_;
	Vector3 dir_;
	float speed_;
	float life_;
};