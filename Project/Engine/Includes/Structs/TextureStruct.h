#pragma once

// C++
#include <cstdint>

// DirectX
#include <d3d12.h>

// DirectxTex
#include "DirectXTex/DirectXTex.h"
#include "DirectX-Headers/include/directx/d3dx12.h"
#include "DirectX/ComPtr/ComPtr.h"

// テクスチャ構造体
struct Texture {
	// テクスチャリソース
	ComPtr<ID3D12Resource> resource;
	// メタデータ
	DirectX::TexMetadata metaData;
	// srvIndex
	uint32_t srvIndex;
};