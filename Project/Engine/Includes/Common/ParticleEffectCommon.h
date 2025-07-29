#pragma once

// C++
#include <cstdint>
#include <string>

// 数学ヘッダ
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// パーティクルエフェクトの発生時データ
/// </summary>
struct ParticleEffectEmitData {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
	Vector3 velocity = { 0.0f,0.0f,0.0f };
	std::string texName = "";
	bool isUseBillboard = true;
	float lifeTime = 1.0f;
};

/// <summary>
/// パーティクルエフェクトのGPUに送る更新用データ
/// </summary>
struct ParticleEffectDataForGPU {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
	Vector3 velocity;
	bool isUseBillboard;
	float lifeTime;
	float currentTime;
};

/// <summary>
/// パーティクルエフェクトのGPUに送る描画用データ
/// </summary>
struct DrawParticleEffectDataForGPU {
	Matrix4x4 worldMat;
	Vector4 color;
	uint32_t texIndex;

	Vector3 padding;
}; 