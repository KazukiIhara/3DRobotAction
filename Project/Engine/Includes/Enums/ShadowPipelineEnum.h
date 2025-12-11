#pragma once

// C++
#include <cstdint>

/// <summary>
/// シャドウ用のパイプラインの種類
/// </summary>
enum class ShadowPipelineStateType {
	Triangle,
	Plane,
	Box,
	Sphere,
	Ring,
	Cylinder,
	Model,

	Num,
};

// パイプラインの種類の数 
static const uint32_t kShadowPipelineStateNum = static_cast<uint32_t>(ShadowPipelineStateType::Num);
