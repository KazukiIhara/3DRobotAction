#pragma once

#include "ViewManagers/BaseViewManager/BaseViewManager.h"

class RTVManager:public BaseViewManager {
public:
	// コンストラクタ
	RTVManager(DXGI* dxgi);
	~RTVManager()override;

	// Texture2d用のRTVの作成
	void CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource,DXGI_FORMAT format);
private:
	// ディスクリプタヒープの作成
	void CreateDescriptorHeap()override;
private:
	// 最大RTV数
	const uint32_t kMaxRTVCount = 64;
};