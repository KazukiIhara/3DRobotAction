#pragma once

#include <cstdint>
#include "Math/Utility/MathUtility.h"

/// <summary>
/// ライトのデータ
/// </summary>
struct PunctualLightData {
	// 全ライト共通
	bool isAlive = true;
	uint32_t type = 0;
	Vector3 color = { 1.0f,1.0f,1.0f };
	float intensity = 1.0f;

	// Spot Point共通
	Vector3 position = { 0.0f,1.0f,0.0f };
	float radius = 10.0f;
	float decay = 5.0f;

	// SpotLight DirectionalLight共通
	Vector3 direction = { 0.0f,-1.0f,0.0f };

	// SpotLight用
	float cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	float cosFalloffStart = 0.1f;
};

/// <summary>
/// GPUに送るライトのデータ
/// </summary>
struct PunctualLightDataForGPU {
	// 全ライト共通
	uint32_t type = 0;
	Vector3 color = { 1.0f,1.0f,1.0f };
	float intensity = 1.0f;

	// Spot Point共通
	Vector3 position = { 0.0f,1.0f,0.0f };
	float radius = 10.0f;
	float decay = 5.0f;

	// SpotLight DirectionalLight共通
	Vector3 direction = { 0.0f,-1.0f,0.0f };

	// SpotLight用
	float cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	float cosFalloffStart = 0.1f;

};

/// <summary>
/// GPUに送るライトの数
/// </summary>
struct PunctualLightCount {
	uint32_t num;
};