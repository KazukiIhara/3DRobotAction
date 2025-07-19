#pragma once

#include "Math/Types/Vector3.h"
#include "Math/Types/Vector4.h"
#include "Math/Types/Matrix4x4.h"

struct DirectionalLightCameraForGPU {
	Matrix4x4 viewProjection;
};

struct DirectionalLight {
	Vector3 direction = { 0.0f,-1.0f,0.0f };
	float intensity = 1.0f;
	Vector3 color = { 1.0f,1.0f,1.0f };
};

struct DirectionalLightForGPU {
	Vector3 direction = { 0.0f,-1.0f,0.0f };
	float intensity = 1.0f;
	Vector3 color = { 1.0f,1.0f,1.0f };
	float padding;
};

struct DirectionalLightFrustumForGPU {
	Vector4 planes[6];   // Left, Right, Bottom, Top, Near, Far
};