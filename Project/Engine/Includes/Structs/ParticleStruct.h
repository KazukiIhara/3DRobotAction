#pragma once

#include "Structs/ObjectStruct.h"

/// <summary>
/// 発生時のパラメータ
/// </summary>
struct EmitParamater {
	Vector3 scale;
	Vector3 rotate;
	Vector3 position;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
};

/// <summary>
/// 描画に使うパラメータ
/// </summary>
struct ParticleData {
	EulerTransform3D transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

/// <summary>
/// GPUに送るパラメータ
/// </summary>
struct ParticleForGPU {
	Matrix4x4 World;
	Vector4 color;
};
