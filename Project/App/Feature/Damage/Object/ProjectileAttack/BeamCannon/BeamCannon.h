#pragma once

// 基底クラス
#include "Feature/Damage/Object/ProjectileAttack/Base/BaseProjectileAttack.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// ビームキャノン
/// </summary>
class BeamCannon:public BaseProjectileAttack {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseProjectileAttack::InitParam initParam_;
	};
public:
	BeamCannon(const BeamCannon::InitParam& initParam, RefContext ref);
	~BeamCannon()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;
private:
	GameEffectManager* effectManager_;

};