#pragma once

#include "Math/Utility/MathUtility.h"

class WorldTransform2D {
public:
	void Initialize();
	void Update();
public:
	// トランスフォーム
	Vector2 size = { 256.0f,256.0f };
	float rotateZ = 0.0f;
	Vector2 position = { 0.0f,0.0f };
	float depth = 0.0f;

	// ワールド行列
	Matrix4x4 worldMatrix{};

};