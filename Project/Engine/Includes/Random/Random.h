#pragma once

#include <cstdint>

#include "Math/Utility/MathUtility.h"

class Random {
public:
	// uint32_t型の乱数を生成
	static uint32_t GenerateUint32_t(uint32_t min, uint32_t max);
	// int32_t型の乱数を生成
	static int32_t GenerateInt32_t(int32_t min, int32_t max);
	// float型の乱数を生成
	static float GenerateFloat(float min, float max);
	// double型の乱数を生成
	static double GenerateDouble(double min, double max);
	// Vector2型の乱数生成
	static Vector2 GenerateVector2(float min, float max);
	// Vector3型の乱数生成
	static Vector3 GenerateVector3(float min, float max);
	// Vector4型の乱数生成
	static Vector4 GenerateVector4(float min, float max);
};