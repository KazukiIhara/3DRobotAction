#pragma once

#include <string>

#include "DirectX-Headers/include/directx/d3dx12_pipeline_state_stream.h"

#include "Math/Utility/MathUtility.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"

/// <summary>
/// プリミティブメッシュシェーダー用のパイプラインストリーム
/// </summary>
struct Primitive3DPipelineStateStream {
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE		rootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_AS					amplificationShader;
	CD3DX12_PIPELINE_STATE_STREAM_MS					meshShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS					pixelShader;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER			rasterizer;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC			blend;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL			depthStencil;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtvFormats;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT	dsvFormat;
};

/// <summary>
/// 3Dの三角形データ
/// </summary>
struct TriangleData3D {
	Vector3 verticesOffsets[3]{
		{ -1.0f,-1.0f,0.0f },
		{ 0.0f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.0f },
	};
};

/// <summary>
/// GPUに送る3Dの三角形データ
/// </summary>
struct TriangleData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	Vector4 Offsets[3];
};

/// <summary>
/// 3D板ポリの形状データ
/// </summary>
struct PlaneData3D {
	Vector3 verticesOffsets[4]{
		{ -1.0f,1.0f,0.0f},
		{ 1.0f,1.0f,0.0f},
		{ -1.0f,-1.0f,0.0f},
		{ 1.0f,-1.0f,0.0f},
	};
};

/// <summary>
/// GPUに送る3Dの板ポリデータ
/// </summary>
struct PlaneData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	Vector4 offsets[4];
};

/// <summary>
/// 3Dボックスの形状データ
/// </summary>
struct BoxData3D {
	Vector3 verticesOffsets[8]{
		{ -1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},
		{ -1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{ -1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},
		{ -1.0f,-1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
	};
};

/// <summary>
/// GPUに送る3Dボックスのデータ
/// </summary>
struct BoxData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	Vector4 offsets[8];
};

/// <summary>
/// 3D球体の形状データ
/// </summary>
struct SphereData3D {
	float radius = 1.0f;
	uint32_t verticalSegments = 16;
	uint32_t horizontalSegments = 16;
};

/// <summary>
/// GPUに送る3Dの球体データ
/// </summary>
struct SphereData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	float radius;
	uint32_t longitudeSegments;
	uint32_t latitudeSegments;
	float padding;
};

/// <summary>
/// 3Dリングの形状データ
/// </summary>
struct RingData3D {
	uint32_t ringDivide = 32;
	float outerRadius = 1.0f;
	float innerRadius = 0.2f;
};

/// <summary>
/// GPUに送る3Dの球体データ
/// </summary>
struct RingData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	uint32_t ringDivide;
	float outerRadius;
	float innerRadius;
	float radianPerDivide;
	float padding[3];
};

/// <summary>
/// 3Dシリンダーの形状データ
/// </summary>
struct CylinderData3D {
	uint32_t divide = 32;
	float topRadius = 1.0f;
	float bottomRadius = 1.0f;
	float height = 3.0f;
	bool hasCap = true;
};

/// <summary>
/// GPUに送るシリンダーの形状データ
/// </summary>
struct CylinderData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	uint32_t divide;
	float topRadius;
	float bottomRadiu;
	float height;
	bool hasCap;
	float padding[3];
};

/// <summary>
/// Primitiveマテリアルデータ
/// </summary>
struct PrimitiveMaterialData3D {
	std::string textureName = "";
	Vector4 baseColor = Color::White;
	Vector2 uvTranslate = { 0.0f,0.0f };
	Vector2 uvScale = { 1.0f,1.0f };
	float uvRotate = 0.0f;
	BlendMode blendMode = BlendMode::None;
};

/// <summary>
/// GPUに送る3DのPrimitiveマテリアルデータ
/// </summary>
struct PrimitiveMaterialData3DForGPU {
	uint32_t textureIndex;
	float padding0[3];
	Vector4 baseColor;
	Matrix4x4 uvMatrix;
};

// 増幅シェーダー用
struct RootConstants {
	uint32_t baseInstanceIndex = 0;
	uint32_t _pad0 = 0;
	uint32_t _pad1 = 0;
	uint32_t _pad2 = 0;
};