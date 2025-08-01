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

struct Camera3DFrustumForGPU {
	Vector4 left;
	Vector4	right;
	Vector4	bottom;
	Vector4	top;
	Vector4	nearClip;
	Vector4 farClip;
};

struct CameraVector {
	Vector3 right;
	float pad0;

	Vector3 up;
	float pad1;

	Vector3 forward;
	float pad2;
};

// シェーダーに送る2Dカメラ情報
struct Camera2DForGPU {
	Matrix4x4 viewProjection;
};