#pragma once

// C++
#include <unordered_map>
#include <optional>

// MyHedder
#include "Structs/ObjectStruct.h"

// ジョイント
struct Joint {
	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};