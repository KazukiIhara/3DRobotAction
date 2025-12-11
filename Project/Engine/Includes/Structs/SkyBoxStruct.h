#pragma once

// C++
#include <cstdint>

// 数学ヘッダ
#include "Math/Types/Vector3.h"
#include "Math/Types/Vector4.h"

#include "Math/Types/Matrix4x4.h"

/// <summary>
/// 背景ボックスの頂点データ
/// </summary>
struct SkyBoxVertexData3D {
	Vector4 position;
};

/// <summary>
/// GPUに送る背景ボックスのデータ
/// </summary>
struct SkyBoxDataForGPU {
	Matrix4x4 worldMatrix;
};