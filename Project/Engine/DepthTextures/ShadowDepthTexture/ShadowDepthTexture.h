#pragma once

#include <cstdint>

#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"


/// <summary>
/// シャドウマップ用の深度テクスチャリソース管理クラス
/// </summary>
class ShadowDepthTexture {
public:
	ShadowDepthTexture();
	~ShadowDepthTexture();

	void Clear();
	void SetAsRenderTarget();
	void TransitionToWrite();
	void TransitionToRead();

	uint32_t GetDsvIndex()const;
	uint32_t GetSrvIndex()const;

public:
	static const uint32_t kShadowMapWidth = 8192;
	static const uint32_t kShadowMapHeight = 8192;

private:
	void CreateResource();

private:
	ComPtr<ID3D12Resource> resource_;
	D3D12_RESOURCE_STATES currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;

	uint32_t dsvIndex_ = 0;
	uint32_t srvIndex_ = 0;
};
