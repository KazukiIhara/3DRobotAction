#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

/// <summary>
/// 発射物の攻撃基底クラス
/// </summary>
class BaseProjectileAttack:public BaseDamageObject {
public:
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float thickness = 0.0f;
		float length = 0.0f;
		float speed = 0.0f;
		float life = 0.0f;
		FriendlyTag tag;
	};
public:
	BaseProjectileAttack(
		const BaseProjectileAttack::InitParam& initParam,
		DamageCollisionSystem* damageCollisionSys
	);
	~BaseProjectileAttack() = default;

	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

private:
	Vector3 dir_;
	float thickness_;
	float length_;
	float speed_;
	float life_;
};