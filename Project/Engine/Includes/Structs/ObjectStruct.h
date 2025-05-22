#pragma once

// C++
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "Structs/ColorStruct.h"

/// <summary>
/// UVトランスフォーム
/// </summary>
struct UVTransform {
	Vector2 scale = { 1.0f,1.0f };
	float rotateZ = 0.0f;
	Vector2 translate = { 0.0f,0.0f };
};

// オイラー角3DTrasform
struct EulerTransform3D {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
};

// クオータニオン角3DTransform
struct QuaternionTransform3D {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Quaternion rotate = { 0.0f,0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
};

// トランスフォーメーション構造体
struct TransformationMatrix {
	Matrix4x4 World;
	Matrix4x4 WorldInverseTransepose;
};

// 3D用マテリアル構造体
struct Material3D {
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	bool enableLighting;
	bool enableSpecularRef;
	UVTransform uvTransform;
	float shininess;
};

// 2D用マテリアル構造体
struct Material2D {
	Vector4 color;
	Matrix4x4 uvTransformMatrix;
};

// GPUに送る3D用マテリアル構造体
struct Material3DForGPU {
	Vector4 color;
	int32_t enableLighting;
	int32_t enableSpecularRef;
	float padding[2];
	Matrix4x4 uvTransformMatrix;
	float shininess;
};

/// <summary>
/// 3Dのラインデータ
/// </summary>
struct LineData3D {
	Vector3 start;
	Vector3 end;
	Vector4 color;
};
