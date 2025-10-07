#pragma once

// C++
#include <list>

// MyHeder
#include "GameEffects/BulletHit/BulletHitParticle/BulletHitParticle.h"

/// <summary>
/// 弾丸ヒット時のエフェクト
/// </summary>
class BulletHitEffect {
public:
	BulletHitEffect();
	~BulletHitEffect() = default;

private:
	std::list<BulletHitParticle> particles_;

};