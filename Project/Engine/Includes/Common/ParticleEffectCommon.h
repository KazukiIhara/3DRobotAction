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
	uint32_t isUseBillboard = 1;
	float lifeTime = 1.0f;
};

/// <summary>
/// パーティクルエフェクトのGPUに送る発生用データ
/// </summary>
struct EmitParticleEffectDataForGPU {
	Vector3 scale;
	float pad0;

	Vector3 rotate;
	float pad1;

	Vector3 translate;
	float pad2;

	Vector3 velocity;
	float pad3;

	bool isUseBillboard;
	Vector3 pad4;

	float lifeTime;
	Vector3 pad5;

	float currentTime;
	Vector3 pad6;
};


/// <summary>
/// パーティクルエフェクトのGPUに送る更新用データ
/// </summary>
struct UpdateParticleEffectDataForGPU {
	Vector3 scale;
	float pad0;

	Vector3 rotate;
	float pad1;

	Vector3 translate;
	float pad2;

	Vector3 velocity;
	float pad3;

	Vector4 color;

	uint32_t texIndex;
	Vector3 pad4;

	bool isUseBillboard;
	Vector3 pad5;

	float lifeTime;
	Vector3 pad6;

	float currentTime;
	Vector3 pad7;
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