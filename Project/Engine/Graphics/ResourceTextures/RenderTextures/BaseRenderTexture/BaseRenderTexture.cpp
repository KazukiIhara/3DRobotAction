#include "BaseRenderTexture.h"

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

BaseRenderTexture::BaseRenderTexture() {

}

BaseRenderTexture::~BaseRenderTexture() {

}

void BaseRenderTexture::Create(DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlags, Vector4 clearColor) {
	format_ = format;
	resourceFlags_ = resourceFlags;
	clearColor_ = clearColor;
	// リソースを作成
	CreateResource();
	// RTVを作成
	CreateRTV();
	// SRVを作成
	CreateSRV();
}

uint32_t BaseRenderTexture::GetRtvIndex() const {
	return rtvIndex_;
}

uint32_t BaseRenderTexture::GetSrvIndex() const {
	return srvIndex_;
}


ID3D12Resource* BaseRenderTexture::GetResource() {
	return resource_.Get();
}

void BaseRenderTexture::TransitionToWrite() {
	// 現在の状態を確認
	if (currentResourceState_ != D3D12_RESOURCE_STATE_RENDER_TARGET) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = resource_.Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		MAGISYSTEM::GetDirectXCommandList()->ResourceBarrier(1, &barrier);

		// 状態を更新
		currentResourceState_ = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}
}

void BaseRenderTexture::TransitionToRead() {
	// 現在の状態を確認
	if (currentResourceState_ != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = resource_.Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		MAGISYSTEM::GetDirectXCommandList()->ResourceBarrier(1, &barrier);

		// 状態を更新
		currentResourceState_ = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	}
}

void BaseRenderTexture::SetAsRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE dsv) {
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorHandle = MAGISYSTEM::GetRTVDescriptorHandleCPU(rtvIndex_);
	if (dsv.ptr == 0) {
		MAGISYSTEM::GetDirectXCommandList()->OMSetRenderTargets(1, &rtvDescriptorHandle, FALSE, nullptr);
	} else {
		MAGISYSTEM::GetDirectXCommandList()->OMSetRenderTargets(1, &rtvDescriptorHandle, FALSE, &dsv);
	}
}

void BaseRenderTexture::ClearRenderTarget() {
	const float clear[] = { clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w };
	MAGISYSTEM::GetDirectXCommandList()->ClearRenderTargetView(MAGISYSTEM::GetRTVDescriptorHandleCPU(rtvIndex_), clear, 0, nullptr);
}

void BaseRenderTexture::CreateResource() {
	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(WindowApp::kClientWidth);					// Textureの幅
	resourceDesc.Height = UINT(WindowApp::kClientHeight);				// Textureの高さ
	resourceDesc.Format = format_;										// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;									// サンプリングカウント。1固定
	resourceDesc.Flags = resourceFlags_;								// renderTargetとして利用可能にする
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// クリアカラーの設定
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format_;
	clearValue.Color[0] = clearColor_.x;
	clearValue.Color[1] = clearColor_.y;
	clearValue.Color[2] = clearColor_.z;
	clearValue.Color[3] = clearColor_.w;

	// リソースの作成
	HRESULT hr_ = S_FALSE;
	resource_ = nullptr;
	hr_ = MAGISYSTEM::GetDirectXDevice()->CreateCommittedResource(
		&heapProperties,								// Heapの設定
		D3D12_HEAP_FLAG_NONE,							// Heapの特殊な設定。特になし。
		&resourceDesc,									// リソースの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET,				// これから描画することを前提としたTextureなので、RenderTargetとして使うことから始める
		&clearValue,									// Clear最適値、ClearRenderをこの色でクリアするようにする、最適化されているので高速である。
		IID_PPV_ARGS(&resource_)
	);
	assert(SUCCEEDED(hr_));
}

void BaseRenderTexture::CreateRTV() {
	// インデックスを割り当て
	rtvIndex_ = MAGISYSTEM::RTVAllocate();
	// RTVを作成
	MAGISYSTEM::CreateRTVTexture2d(rtvIndex_, resource_.Get(), format_);
}

void BaseRenderTexture::CreateSRV() {
	// インデックス割り当て
	srvIndex_ = MAGISYSTEM::SrvUavAllocate();
	// SRVを作成
	MAGISYSTEM::CreateSrvTexture2D(srvIndex_, resource_.Get(), format_, 1);
}

