#pragma once

// C++
#include <string>
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// 3Dプリミティブの種類
/// </summary>
enum class Prim3DType {
	Triangle,
	Plane,
	Box,
	Sphere,
};

/// <summary>
/// 3Dプリミティブ描画
/// </summary>
class PrimitiveRenderer3D {
public:
	PrimitiveRenderer3D(const std::string& name, const Prim3DType& type);
	~PrimitiveRenderer3D();

	void Draw();

	Transform3D* GetTransform();

private:
	// 名前
	std::string name_ = "";
	// プリミティブタイプ
	Prim3DType type_ = Prim3DType::Triangle;
	// トランスフォーム
	Transform3D* transform_ = nullptr;
	// マテリアル
	MaterialData3D material_{};
	// 描画フラグ
	bool isRender_ = true;

	// 各形状のデータ
	TriangleData3D triangleData_{};
	PlaneData3D planeData_{};
	BoxData3D boxData_{};
	SphereData3D sphereData_{};

};
