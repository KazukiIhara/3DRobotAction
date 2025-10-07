#pragma once

// C++
#include <list>
#include <memory>

// MyHeder
#include "GameEffects/BulletHit/BulletHitParticle/BulletHitParticle.h"

class MechCore;

/// <summary>
/// 弾丸ヒット時のエフェクト
/// </summary>
class BulletHitEffect {
public:
	BulletHitEffect(MechCore* mechCore);
	~BulletHitEffect() = default;

	void Emit(const Vector3& pos);

	void Update();

	void Draw();

private:
	void EmitParticle(const Vector3& pos);

private:
	// 機体のポインタ
	MechCore* mech_ = nullptr;

	// パーティクル
	std::list<std::unique_ptr<BulletHitParticle>> bulletHitParticles_;

	// 一度に発生するパーティクルの数
	uint32_t emitNum_ = 30;


	// 
	// パラメータ
	// 
	float diffusionRange_ = 0.25f;
	float gravity_ = -2.0f;
};