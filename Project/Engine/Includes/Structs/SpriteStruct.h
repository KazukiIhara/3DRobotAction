#pragma once

// C++
#include <cstdint>
#include <string>

// DirectX
#include <d3d12.h>

#include "DirectX-Headers/include/directx/d3dx12_pipeline_state_stream.h"

// 数学ヘッダ
#include "Math/Types/Vector2.h"
#include "Math/Types/Vector4.h"
#include "Math/Types/Matrix4x4.h"

#include "Enums/BlendModeEnum.h"

/// <summary>
/// スプライトメッシュシェーダー用のパイプラインストリーム
/// </summary>
struct SpritePipelineStateStream {
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE		rootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_MS					meshShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS					pixelShader;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER			rasterizer;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC			blend;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL			depthStencil;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtvFormats;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT	dsvFormat;
};

/// <summary>
/// CPU側で使うスプライトのデータ
/// </summary>
struct SpriteData {
	Vector2 size = { -1.0f,-1.0f };
	float rotate = 0.0f;
	Vector2 position = { 0.0f,0.0f };
	bool isBack = false; // 背景スプライトかどうか
};

/// <summary>
/// CPU側で使うスプライトのマテリアルデータ
/// </summary>
struct SpriteMaterialData {
	std::string textureName = "";
	BlendMode blendmode = BlendMode::Normal;
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.0f,0.0f };
	Vector2 textureLeftTop = { 0.0f,0.0f };
	Vector2 textureCutOutSize = { -1.0f,-1.0f };

	bool isFlipX = false;
	bool isFlipY = false;
	Vector2 uvScale = { 1.0f,1.0f };
	float uvRotate = 0.0f;
	Vector2 uvTranslate = { 0.0f,0.0f };
};

/// <summary>
/// GPUに送るSpriteのデータ
/// </summary>
struct SpriteDataForGPU {
	// 
	// 座標や形状
	// 
	Matrix4x4 worldMatrix;
	Vector2 vertexPosition[4];
	Vector2 texcooed[4];

	// 
	// マテリアル
	// 
	uint32_t textureIndex;
	Vector4 baseColor;
	Matrix4x4 uvMatrix;
	float padding0[3];
};