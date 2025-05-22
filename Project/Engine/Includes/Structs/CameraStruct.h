#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"

// シェーダーに送る3Dカメラ情報
struct Camera3DForGPU {
	Matrix4x4 viewProjection;
	Vector3 worldPosition;
	float padding;
};

struct Camera3DInverseForGPU {
	Matrix4x4 invView;
	Matrix4x4 invProj;
};

// シェーダーに送る2Dカメラ情報
struct Camera2DForGPU {
	Matrix4x4 viewProjection;
};