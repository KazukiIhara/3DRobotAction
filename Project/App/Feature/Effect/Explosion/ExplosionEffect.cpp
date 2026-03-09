#include "ExplosionEffect.h"

#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "MAGI.h"

#include "Random/Random.h"

using namespace Magi;

ExplosionEffect::ExplosionEffect(const Vector3& emitPos) :
	BaseGameEffect(emitPos) {

	// テクスチャロード
	MAGISYSTEM::LoadTexture("smoke.png");
	MAGISYSTEM::LoadTexture("fire_01.png");

	// パラメータ作成

	// 爆発パーティクルの数
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticleNum" }, ParamType::Int32);
	// 爆発パーティクルの色
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticleColor" }, ParamType::Vec4);
	// 爆発パーティクルの発生半径
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticlePosRadius" }, ParamType::Float);
	// 爆発パーティクルの寿命
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticleLife" }, ParamType::Float);
	// 爆発パーティクルのサイズ
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticleSize" }, ParamType::Float);
	// 爆発パーティクルの最低速度
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticleSpeedMin" }, ParamType::Float);
	// 爆発パーティクルの最高速度
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","ExplosionParticleSpeedMax" }, ParamType::Float);

	// 煙パーティクルの数
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticleNum" }, ParamType::Int32);
	// 煙パーティクルの色
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticleColor" }, ParamType::Vec4);
	// 煙パーティクルの発生半径
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticlePosRadius" }, ParamType::Float);
	// 煙パーティクルの寿命
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticleLife" }, ParamType::Float);
	// 煙パーティクルのサイズ
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticleSize" }, ParamType::Float);
	// 煙パーティクルの最低速度
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticleSpeedMin" }, ParamType::Float);
	// 煙パーティクルの最高速度
	MAGISYSTEM::AddParameterData({ "EffectParam","Explosion","SmokeParticleSpeedMax" }, ParamType::Float);

	// ゼロベクトルを避けてランダム方向を作る
	auto randomDir = []() -> Vector3 {
		while (true) {
			Vector3 dir = Random::GenerateVector3(-100.0f, 100.0f);
			if (Dot(dir, dir) > 1e-6f) {
				return Normalize(dir);
			}
		}
		};

	// 座標をセット
	worldPos_ = emitPos;

	// 爆発パーティクル発生

	// 爆発パーティクルの設定を取得
	const int32_t explosionParticleNum = MAGISYSTEM::GetParameterValue<int32_t>({ "EffectParam","Explosion","ExplosionParticleNum" });
	const Vector4 explosionParticleColor = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","Explosion","ExplosionParticleColor" });
	const float explosionParticlePosRadius = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","ExplosionParticlePosRadius" });
	const float explosionParticleLife = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","ExplosionParticleLife" });
	const float explosionParticleSize = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","ExplosionParticleSize" });
	const float explosionParticleSpeedMin = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","ExplosionParticleSpeedMin" });
	const float explosionParticleSpeedMax = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","ExplosionParticleSpeedMax" });

	// 爆発パーティクルのテクスチャを取得
	const int32_t explosionParticleTex = MAGISYSTEM::GetTextureIndex("fire_01.png");

	// 爆発パーティクルを発生
	for (int32_t i = 0; i < explosionParticleNum; i++) {

		// 発生位置のオフセットを作る
		const Vector3 offset = randomDir() * Random::GenerateFloat(0.0f, explosionParticlePosRadius);
		// 速度方向を作る
		const Vector3 dir = randomDir();
		// 速度を決める
		const float speed = Random::GenerateFloat(explosionParticleSpeedMin, explosionParticleSpeedMax);

		// パーティクルデータを作る
		GPUParticleEmitData particleData;
		particleData.texIndex = explosionParticleTex;
		particleData.pos = worldPos_ + offset;
		particleData.velo = dir * speed;
		particleData.size = Vector2(explosionParticleSize, explosionParticleSize);
		particleData.color = explosionParticleColor;
		particleData.life = explosionParticleLife;

		// パーティクルを発生
		MAGISYSTEM::EmitParticle(particleData);
	}

	// 煙パーティクル発生

	// 煙パーティクルの設定を取得
	const int32_t smokeParticleNum = MAGISYSTEM::GetParameterValue<int32_t>({ "EffectParam","Explosion","SmokeParticleNum" });
	const Vector4 smokeParticleColor = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","Explosion","SmokeParticleColor" });
	const float smokeParticlePosRadius = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","SmokeParticlePosRadius" });
	const float smokeParticleLife = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","SmokeParticleLife" });
	const float smokeParticleSize = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","SmokeParticleSize" });
	const float smokeParticleSpeedMin = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","SmokeParticleSpeedMin" });
	const float smokeParticleSpeedMax = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Explosion","SmokeParticleSpeedMax" });

	// 煙パーティクルのテクスチャを取得
	const int32_t smokeParticleTex = MAGISYSTEM::GetTextureIndex("smoke.png");

	// 煙パーティクルを発生
	for (int32_t i = 0; i < smokeParticleNum; i++) {

		// 発生位置のオフセットを作る
		const Vector3 offset = randomDir() * Random::GenerateFloat(0.0f, smokeParticlePosRadius);

		// 上方向寄りの移動方向を作る
		Vector3 dir = randomDir();
		dir.y = std::abs(dir.y) + 0.5f;
		dir = Normalize(dir);

		// 速度を決める
		const float speed = Random::GenerateFloat(smokeParticleSpeedMin, smokeParticleSpeedMax);

		// パーティクルデータを作る
		GPUParticleEmitData particleData;
		particleData.texIndex = smokeParticleTex;
		particleData.pos = worldPos_ + offset;
		particleData.velo = dir * speed;
		particleData.size = Vector2(smokeParticleSize, smokeParticleSize);
		particleData.color = smokeParticleColor;
		particleData.life = smokeParticleLife;

		// パーティクルを発生
		MAGISYSTEM::EmitParticle(particleData);
	}



}

void ExplosionEffect::Update() {
	isAlive_ = false;
}

void ExplosionEffect::Draw() {

}
