#pragma once

#include "ViewManagers/BaseViewManager/BaseViewManager.h"

class SRVUAVManager:public BaseViewManager {
public:
	SRVUAVManager(DXGI* dxgi);
	~SRVUAVManager()override;
	// SRV生成(テクスチャ用)
	void CreateSrvTexture2d(uint32_t viewIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	// SRV生成(キューブマップ用)
	void CreateSrvTextureCubeMap(uint32_t viewIndex, ID3D12Resource* pResource, DXGI_FORMAT format);
	// SRV生成(StructuredBuffer用)
	void CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
	// SRV作成(RAWBuffer用)
	void CreateSrvByteAddressBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t byteSize);
	// UAV作成
	void CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
private:
	// ディスクリプタヒープの作成
	void CreateDescriptorHeap()override;
private:
	// 最大SRV数
	const uint32_t kMaxViewCount_ = 65536;
};