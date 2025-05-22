#include "ShadowDepthTexture.h"

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"


using namespace MAGIUtility;

ShadowDepthTexture::ShadowDepthTexture() {
	CreateResource();
}

ShadowDepthTexture::~ShadowDepthTexture() {}

void ShadowDepthTexture::CreateResource() {

	// リソース作成
	resource_ = MAGISYSTEM::CreateDepthStencilTextureResource(
		kShadowMapWidth,
		kShadowMapHeight,
		DXGI_FORMAT_D32_FLOAT
	);

	// ビュー作成
	dsvIndex_ = MAGISYSTEM::DSVAllocate();
	srvIndex_ = MAGISYSTEM::SrvUavAllocate();

	MAGISYSTEM::CreateDSVTexture2d(dsvIndex_, resource_.Get(), DXGI_FORMAT_D32_FLOAT);
	MAGISYSTEM::CreateSrvTexture2D(srvIndex_, resource_.Get(), DXGI_FORMAT_R32_FLOAT, 1);

	currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
}

void ShadowDepthTexture::Clear() {
	MAGISYSTEM::GetDirectXCommandList()->ClearDepthStencilView(
		MAGISYSTEM::GetDSVDescriptorHandleCPU(dsvIndex_),
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);
}

void ShadowDepthTexture::SetAsRenderTarget() {
	auto dsvHandle = MAGISYSTEM::GetDSVDescriptorHandleCPU(dsvIndex_);
	MAGISYSTEM::GetDirectXCommandList()->OMSetRenderTargets(0, nullptr, FALSE, &dsvHandle);
}

void ShadowDepthTexture::TransitionToWrite() {
	if (currentResourceState_ != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = resource_.Get();
		barrier.Transition.StateBefore = currentResourceState_;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		MAGISYSTEM::GetDirectXCommandList()->ResourceBarrier(1, &barrier);

		currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	}
}

void ShadowDepthTexture::TransitionToRead() {
	if (currentResourceState_ != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = resource_.Get();
		barrier.Transition.StateBefore = currentResourceState_;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		MAGISYSTEM::GetDirectXCommandList()->ResourceBarrier(1, &barrier);

		currentResourceState_ = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	}
}

uint32_t ShadowDepthTexture::GetDsvIndex() const {
	return dsvIndex_;
}

uint32_t ShadowDepthTexture::GetSrvIndex() const {
	return srvIndex_;
}
