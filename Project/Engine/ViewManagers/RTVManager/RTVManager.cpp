#include "RTVManager.h"

#include "DirectX/DXGI/DXGI.h"

#include "Logger/Logger.h"

RTVManager::RTVManager(DXGI* dxgi) :BaseViewManager(dxgi) {
	// 基底クラスの初期化処理
	BaseViewManager::Initialize(kMaxRTVCount);
	Logger::Log("RTVManager Initialize\n");
}
RTVManager::~RTVManager() {
	Logger::Log("RTVManager Finalize\n");
}

void RTVManager::CreateDescriptorHeap() {
	// デスクリプタヒープの作成
	descriptorHeap_ = dxgi_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxRTVCount, false);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize_ = dxgi_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void RTVManager::CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {
	// rtvDesc
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む
	dxgi_->GetDevice()->CreateRenderTargetView(pResource, &rtvDesc, GetDescriptorHandleCPU(rtvIndex));
}
