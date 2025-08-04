#pragma once

// C++
#include <cstdint>
#include <string>

// 数学ヘッダ
#include "Math/Types/AllMathTypes.h"

// DirectX
#include <d3d12.h>

#include "DirectX-Headers/include/directx/d3dx12_pipeline_state_stream.h"

/// <summary>
/// パーティクル用のパイプラインストリーム
/// </summary>
struct Particle3DPipelineStateStream {
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

// パーティクルの最大数
static constexpr uint32_t kMaxParticleNum = 1024;

/// <summary>
/// パーティクルエフェクトの発生時データ
/// </summary>
struct GPUParticleEmitData {
	Vector3 pos = { 0.0f,0.0f,0.0 };
	float life = 1.0f;

	Vector3 velo = { 0.0f,0.0f,0.0f };
	uint32_t texIndex = 1;

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	Vector2 size = { 1.0f,1.0f };
	Vector2 pad0;
};

/// <summary>
/// パーティクル構造体
/// </summary>
struct GPUParticle {
	Vector3 pos;
	float life;

	Vector3 velo;
	uint32_t texIdx;

	Vector4 color;

	Vector2 size;
	float age;
	float pad0;
};

/// <summary>
/// 発生カウント用
/// </summary>
struct GPUParticleInfo {
	uint32_t emitCount;
	float deltaTime;
	Vector2 pad0;
};