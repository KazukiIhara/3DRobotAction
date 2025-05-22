#pragma once

#include "Math/Utility/MathUtility.h"
#include "Structs/ObjectStruct.h"

struct GameObject3DData {
	EulerTransform3D transform;
};

struct GameObject3DDataForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
};