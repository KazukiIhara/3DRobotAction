#pragma once

// 基底クラス
#include "Feature/Damage/Object/ProjectileAttack/Base/BaseProjectileAttack.h"

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

};
