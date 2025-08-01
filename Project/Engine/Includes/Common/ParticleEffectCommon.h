#pragma once

// C++
#include <cstdint>
#include <string>

// 数学ヘッダ
#include "Math/Types/AllMathTypes.h"

// パーティクルの最大数
static constexpr uint32_t kMaxParticleNum = 1024;

/// <summary>
/// パーティクルエフェクトの発生時データ
/// </summary>
struct GPUParticleEmitData {
	Vector3 pos;
	float life;

	Vector3 velo;
	uint32_t texIndex;

	Vector4 color;

	Vector2 size;
	Vector2 pad0;
};

/// <summary>
/// パーティクル構造体
/// </summary>
struct GPUParticle {
	Vector3 pos;
	float age;

	Vector3 velo;
	float life;

	Vector4 color;

	Vector2 size;
	uint32_t texIdx;
	float pad0;
};