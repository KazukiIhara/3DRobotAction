#pragma once

// MyHedder
#include "magi::Transform3D/magi::Transform3D.h"
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

	void Update();

private:
	// エフェクト発生
	void EmitBodyEffect();

private:
	MechCore* mech_ = nullptr;

	// 
	// パラメータ
	// 

	uint32_t smokeEffectNum_ = 1000;



private:
	// 胴体用エフェクトデータ
	GPUParticleEmitData bodySmokeEffect_;
};