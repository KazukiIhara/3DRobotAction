#pragma once

#include <string>

/// <summary>
/// プリミティブ3Dの種類
/// </summary>
enum class Primitive3DType {
	Plane,
	Sphere,
	Ring,
	Cylinder,
};

inline std::string PrimitiveTypeToString(Primitive3DType type) {
	switch (type) {
	case Primitive3DType::Plane:
		return "Plane";
	case Primitive3DType::Sphere:
		return "Sphere";
	case Primitive3DType::Ring:
		return "Ring";
	case Primitive3DType::Cylinder:
		return "Cylinder";
	default:
		return "Unkown";
	}
}

