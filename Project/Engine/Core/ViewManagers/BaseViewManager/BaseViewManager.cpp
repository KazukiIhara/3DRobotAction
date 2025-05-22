#include "BaseViewManager.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"

BaseViewManager::BaseViewManager(DXGI* dxgi) {
	// DXGIのセット
	SetDXGI(dxgi);
}

void BaseViewManager::Initialize(const uint32_t& maxViewCount) {
	// ディスクリプタヒープ作成
	CreateDescriptorHeap();
	// ビューの最大数を設定
	maxViewCount_ = maxViewCount;
}

uint32_t BaseViewManager::Allocate() {
	uint32_t index = useIndex;
	useIndex++;
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE BaseViewManager::GetDescriptorHandleCPU(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE BaseViewManager::GetDescriptorHandleGPU(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

ID3D12DescriptorHeap* BaseViewManager::GetDescriptorHeap() const {
	return descriptorHeap_.Get();
}

uint32_t BaseViewManager::GetDescriptorSize() const {
	return descriptorSize_;
}

bool BaseViewManager::IsLowerViewMax() const {
	return useIndex < maxViewCount_;
}

void BaseViewManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}
