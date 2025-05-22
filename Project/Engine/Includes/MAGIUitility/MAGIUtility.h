#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>
#include <array>
#include <cstdint>

// MyHedder
#include "Math/Utility/MathUtility.h"

#include "Structs/AnimationStruct.h"
#include "Structs/ColorStruct.h"

/// <summary>
/// 便利関数
/// </summary>
namespace MAGIUtility {

	constexpr uint32_t AS_GROUP_SIZE = 32;

	// Vector3のキーフレーム補間
	Vector3 CalculateVelue(const std::vector<KeyframeVector3>& keyframes, float time);
	// Quaternionのキーフレーム補間
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	// ガウスの重みを生成する
	std::array<float, 7> GenerateGaussianWeights(float sigma);

	uint32_t DivRoundUp(uint32_t num, uint32_t den);

}