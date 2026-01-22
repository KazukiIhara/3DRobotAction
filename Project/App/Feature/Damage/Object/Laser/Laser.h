#pragma once

// 基底クラス
#include "Feature/Damage/Object/LinearAttack/Base/BaseLinearAttack.h"

// レーザーエフェクト
#include "Feature/Effect/LaserEffect/LaserEffect.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// レーザー
/// </summary>
class Laser :public BaseLinearAttack {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseLinearAttack::InitParam initParam_;
		LaserEffect::Col color;
	};
public:
	Laser(const Laser::InitParam& initParam, RefContext ref);
	~Laser()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

private:
	GameEffectManager* effectManager_;
	LaserEffect* laserEffect_;
};