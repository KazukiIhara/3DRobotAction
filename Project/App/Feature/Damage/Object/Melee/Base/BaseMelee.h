#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

/// <summary>
/// 近接攻撃基底クラス
/// </summary>
class BaseMelee:public BaseDamageObject {
public:
	struct InitParam {
		Magi::Transform3D* parent;
		float radius = 0.0f;
		float life = 0.0f;
		DamageCollider::GameParam gParam;
	};
public:
	BaseMelee(
		const BaseMelee::InitParam& initParam,
		DamageCollisionSystem* dmgColSystem
	);
	~BaseMelee() = default;

	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

	float GetRadius()const;
	float GetLife()const;

private:
	Magi::Transform3D* parent_;
	float radius_;
	float life_;
};