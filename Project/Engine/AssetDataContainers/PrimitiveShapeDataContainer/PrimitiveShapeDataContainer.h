#pragma once

// C++
#include <unordered_map>

// MyHedder
#include "Enums/Primitive3DEnum.h"
#include "Structs/ModelStruct.h"

/// <summary>
/// シンプル形状のデータコンテナクラス
/// </summary>
class PrimitiveShapeDataContainer {
public:
	PrimitiveShapeDataContainer();
	~PrimitiveShapeDataContainer();
	PrimitiveData GetPrimitiveShapeData(const Primitive3DType& primitiveType);
private:
	void Initialize();

	void CreatePlane();
	void CreateSphere();
private:
	// シンプル形状のデータ
	std::unordered_map<Primitive3DType, PrimitiveData> primitiveDatas_;
};