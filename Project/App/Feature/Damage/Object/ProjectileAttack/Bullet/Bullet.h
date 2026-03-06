#pragma once

// C++
#include <array>


// 基底クラス
#include "Feature/Damage/Object/ProjectileAttack/Base/BaseProjectileAttack.h"

#include "Structs/Primitive3DStruct.h"

// 弾ヒット時エフェクト
#include "Feature/Effect/MachineGunHit/MachineGunHitEffect.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// 実弾
/// </summary>
class Bullet:public BaseProjectileAttack {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseProjectileAttack::InitParam initParam_;
	};
public:
	Bullet(const Bullet::InitParam& initParam, RefContext ref);
	~Bullet() = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

private:
	GameEffectManager* effectManager_;

	Transform3D* bulletTransParent_;
	std::array<Transform3D*, 2> bulletTrans_;
	std::array<PlaneData3D, 2> bulletPlane_;
	MaterialData3D bulletPlaneMat_;

	Vector3 effectScale_;
	MaterialData3D effectMat_;
};
