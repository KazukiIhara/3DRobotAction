#pragma once

// 基底クラス
#include "Feature/Damage/Object/ProjectileAttack/Base/BaseProjectileAttack.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// エネルギーっぽい弾
/// </summary>
class Beam:public BaseProjectileAttack {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseProjectileAttack::InitParam initParam_;
	};
public:
	Beam(const Beam::InitParam& initParam, RefContext ref);
	~Beam() = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

private:
	GameEffectManager* effectManager_;

};
