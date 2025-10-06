#pragma once

// MyHedder
#include "Transform3D/Transform3D.h"
#include "Common/Particle/ParticleEffectCommon.h"

// 前方宣言
class MechCore;

/// <summary>
/// クイックブースト時のパーティクルエフェクト
/// </summary>
class QuickBoostParticle {
public:
	QuickBoostParticle(MechCore* mechCore);
	~QuickBoostParticle() = default;

	void Emit();

private:
	void EmitBodyEffect();

private:
	MechCore* mech_ = nullptr;

	// 
	// パラメータ
	// 

	uint32_t smokeEffectNum_ = 300;
	uint32_t particleEffectNum_ = 2500;
private:
	// 胴体用エフェクトデータ
	GPUParticleEmitData bodySmokeEffect_;
	GPUParticleEmitData particleEffect_;
};