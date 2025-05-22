#pragma once

// C++
#include <cstdint>

/// <summary>
/// グラフィックスパイプラインの種類
/// </summary>
enum class GraphicsPipelineStateType {
	Object2D,
	Sprite,

	Line3D,
	Triangle3D,
	Plane3D,
	Box3D,
	Sphere3D,
	Ring3D,
	Cylinder3D,
	Model3D,
	SkyBox,

	Object3D,
	Particle3D,
	Object3DGroup,

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kGraphicsPipelineStateNum = static_cast<uint32_t>(GraphicsPipelineStateType::Num);
