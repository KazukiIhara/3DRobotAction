#include "BossDestroyEffect.h"

#include <algorithm>
#include <cmath>

#include "Feature/Effect/Explosion/ExplosionEffect.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "MAGI.h"

#include "Random/Random.h"

using namespace Magi;
using namespace MAGIMath;

namespace {

	// ランダムな単位ベクトルを作る
	Vector3 GenerateRandomUnitVector() {
		while (true) {
			Vector3 dir = Random::GenerateVector3(-100.0f, 100.0f);
			if (Dot(dir, dir) > 1e-6f) {
				return Normalize(dir);
			}
		}
	}

	// 次の爆発までの時間を決める
	float GenerateExplosionInterval(float minInterval, float maxInterval) {
		if (maxInterval < minInterval) {
			std::swap(minInterval, maxInterval);
		}

		return Random::GenerateFloat(minInterval, maxInterval);
	}

	// 1回で発生する爆発数を決める
	int32_t GenerateExplosionCount(int32_t minCount, int32_t maxCount) {
		if (maxCount < minCount) {
			std::swap(minCount, maxCount);
		}

		return Random::GenerateInt32_t(minCount, maxCount);
	}

	// 爆発位置オフセットを作る
	Vector3 GenerateExplosionOffset(
		float radiusMin,
		float radiusMax,
		float yOffsetMin,
		float yOffsetMax) {

		// 半径の大小関係を補正
		if (radiusMax < radiusMin) {
			std::swap(radiusMin, radiusMax);
		}

		// 高さの大小関係を補正
		if (yOffsetMax < yOffsetMin) {
			std::swap(yOffsetMin, yOffsetMax);
		}

		// 水平方向のランダム方向を作る
		Vector3 dir = GenerateRandomUnitVector();
		dir.y = 0.0f;

		// 水平方向がゼロなら作り直す
		if (Dot(dir, dir) <= 1e-6f) {
			dir = Vector3(1.0f, 0.0f, 0.0f);
		} else {
			dir = Normalize(dir);
		}

		// 中心からの距離を決める
		const float radius = Random::GenerateFloat(radiusMin, radiusMax);
		// 高さ方向のずれを決める
		const float yOffset = Random::GenerateFloat(yOffsetMin, yOffsetMax);

		Vector3 offset = dir * radius;
		offset.y = yOffset;

		return offset;
	}
}

BossDestroyEffect::BossDestroyEffect(const Vector3& emitPos, GameEffectManager* gameEffectManager) :
	BaseGameEffect(emitPos) {

	gameEffectManager_ = gameEffectManager;

	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","Time" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionCountMin" }, ParamType::Int32);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionCountMax" }, ParamType::Int32);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionIntervalMin" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionIntervalMax" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionRadiusMin" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionRadiusMax" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionYOffsetMin" }, ParamType::Float);
	MAGISYSTEM::AddParameterData({ "EffectParam","BossDestroy","ExplosionYOffsetMax" }, ParamType::Float);

	// タイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","Time" });

	// 最初の爆発までのタイマーを初期化
	explosionTimer_ = 0.0f;
}

void BossDestroyEffect::Update() {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 生存タイマー更新
	time_ -= dt;
	if (time_ <= 0.0f) {
		isAlive_ = false;
		return;
	}

	// 爆発タイマー更新
	explosionTimer_ -= dt;
	if (explosionTimer_ > 0.0f) {
		return;
	}

	// 爆発数設定を取得
	const int32_t explosionCountMin = MAGISYSTEM::GetParameterValue<int32_t>({ "EffectParam","BossDestroy","ExplosionCountMin" });
	const int32_t explosionCountMax = MAGISYSTEM::GetParameterValue<int32_t>({ "EffectParam","BossDestroy","ExplosionCountMax" });

	// 爆発間隔設定を取得
	const float explosionIntervalMin = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","ExplosionIntervalMin" });
	const float explosionIntervalMax = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","ExplosionIntervalMax" });

	// 爆発半径設定を取得
	const float explosionRadiusMin = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","ExplosionRadiusMin" });
	const float explosionRadiusMax = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","ExplosionRadiusMax" });

	// 爆発の高さ設定を取得
	const float explosionYOffsetMin = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","ExplosionYOffsetMin" });
	const float explosionYOffsetMax = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossDestroy","ExplosionYOffsetMax" });

	// 今回発生させる爆発数を決める
	const int32_t explosionCount = GenerateExplosionCount(explosionCountMin, explosionCountMax);

	// 複数の爆発を発生
	for (int32_t i = 0; i < explosionCount; i++) {

		// 爆発位置オフセットを作る
		const Vector3 offset = GenerateExplosionOffset(
			explosionRadiusMin,
			explosionRadiusMax,
			explosionYOffsetMin,
			explosionYOffsetMax);

		// 爆発エフェクトを追加
		gameEffectManager_->Add(std::move(std::make_unique<ExplosionEffect>(worldPos_ + offset)));
	}

	// 次の爆発タイマーを設定
	explosionTimer_ = GenerateExplosionInterval(explosionIntervalMin, explosionIntervalMax);
}

void BossDestroyEffect::Draw() {

}