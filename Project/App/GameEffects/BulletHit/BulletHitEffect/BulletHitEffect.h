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

	void Emit(const Vector3& pos);

	void Update();

	void Draw();

private:
	void EmitParticle(const Vector3& pos);

private:
	// パーティクル
	std::list<BulletHitParticle> bulletHitParticles_;

};