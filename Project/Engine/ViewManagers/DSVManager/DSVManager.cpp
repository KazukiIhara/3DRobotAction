#include "DSVManager.h"

#include "DirectX/DXGI/DXGI.h"

#include "Logger/Logger.h"

DSVManager::DSVManager(DXGI* dxgi) :BaseViewManager(dxgi) {
	// 基底クラスの初期化処理
	BaseViewManager::Initialize(kMaxDSVCount_);
	Logger::Log("DSVManager Initialize\n");
}

DSVManager::~DSVManager() {
	Logger::Log("DSVManager Finalize\n");
}

void DSVManager::CreateDescriptorHeap() {
	// ディスクリプタヒープの作成
	descriptorHeap_ = dxgi_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kMaxDSVCount_, false);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize_ = dxgi_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void DSVManager::CreateDSVTexture2d(uint32_t dsvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {
	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	// DSV作成
	dxgi_->GetDevice()->CreateDepthStencilView(pResource, &dsvDesc, GetDescriptorHandleCPU(dsvIndex));
}
