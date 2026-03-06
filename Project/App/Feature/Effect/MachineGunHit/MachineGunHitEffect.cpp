#include "MachineGunHitEffect.h"

#include "MAGI.h"

#include "Random/Random.h"

using namespace Magi;

MachineGunHitEffect::MachineGunHitEffect(const Vector3& emitPos) :
	BaseGameEffect(emitPos) {

	// パラメータ作成

	// エフェクトの時間
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","EffectTime" }, ParamType::Float);
	// リングの開始サイズ
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","RingStartSize" }, ParamType::Float);
	// リングの終了時サイズ
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","RingEndSize" }, ParamType::Float);

	// パーティクルの数
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleCount" }, ParamType::Int32);
	// 発生位置の拡散
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleEmitSpread" }, ParamType::Float);
	// パーティクルの色
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleColor" }, ParamType::Vec4);
	// パーティクルの基準方向
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleBaseDir" }, ParamType::Vec3);
	// パーティクルの拡散率
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleSpread" }, ParamType::Float);
	// パーティクルの生存時間
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleLife" }, ParamType::Float);
	// パーティクルのサイズ
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleSize" }, ParamType::Float);
	// パーティクルの速度
	MAGISYSTEM::AddParameterData({ "EffectParam","MachineGunHit","ParticleSpeed" }, ParamType::Float);


	// テクスチャロード

	// ヒットエフェクト用
	MAGISYSTEM::LoadTexture("gradationLine.png");

	// 煙パーティクル用
	MAGISYSTEM::LoadTexture("smoke.png");

	// エフェクトのタイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","MachineGunHit","EffectTime" });

	// 座標をセット
	ringTrans_ = emitPos;

	// パーティクル

	// パラメータを取得
	const int32_t particleTex = MAGISYSTEM::GetTextureIndex("smoke.png");
	const int32_t particleNum = MAGISYSTEM::GetParameterValue<int32_t>({ "EffectParam","MachineGunHit","ParticleCount" });
	const Vector4 particleColor = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","MachineGunHit","ParticleColor" });
	const Vector3 particleBaseDir = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","MachineGunHit","ParticleBaseDir" });
	const float particleEmitSpread = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","MachineGunHit","ParticleEmitSpread" });
	const float particleSpread = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","MachineGunHit","ParticleSpread" });
	const float particleLife = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","MachineGunHit","ParticleLife" });
	const float particleSize = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","MachineGunHit","ParticleSize" });
	const float particleSpeed = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","MachineGunHit","ParticleSpeed" });

	// 基準方向を正規化
	Vector3 baseDir = particleBaseDir;
	if (Dot(baseDir, baseDir) <= 1.0e-6f) {
		baseDir = Vector3(0.0f, 1.0f, 0.0f);
	} else {
		baseDir = Normalize(baseDir);
	}

	// 発生
	for (int32_t i = 0; i < particleNum; i++) {

		// ランダム方向を作成
		Vector3 randomDir = Random::GenerateVector3(-1.0f, 1.0f);
		if (Dot(randomDir, randomDir) <= 1.0e-6f) {
			randomDir = baseDir;
		} else {
			randomDir = Normalize(randomDir);
		}

		// 基準方向から拡散
		Vector3 dir = baseDir + randomDir * particleSpread;
		if (Dot(dir, dir) <= 1.0e-6f) {
			dir = baseDir;
		} else {
			dir = Normalize(dir);
		}

		// 発生位置ランダム
		Vector3 emitOffset = Random::GenerateVector3(-1.0f, 1.0f);
		if (Dot(emitOffset, emitOffset) > 1.0e-6f) {
			emitOffset = Normalize(emitOffset);
		}
		emitOffset *= particleEmitSpread;

		GPUParticleEmitData particleData{};
		particleData.texIndex = particleTex;
		particleData.pos = worldPos_ + emitOffset;
		particleData.velo = dir * particleSpeed;
		particleData.size = Vector2(particleSize, particleSize);
		particleData.color = particleColor;
		particleData.life = particleLife;

		MAGISYSTEM::EmitParticle(particleData);
	}

}

void MachineGunHitEffect::Update() {

}

void MachineGunHitEffect::Draw() {

}
