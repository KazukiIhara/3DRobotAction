#include "SRVUAVManager.h"

#include "DirectX/DXGI/DXGI.h"

#include "Logger/Logger.h"

SRVUAVManager::SRVUAVManager(DXGI* dxgi) :BaseViewManager(dxgi) {
	// 基底クラスの初期化処理
	BaseViewManager::Initialize(kMaxViewCount_);
	Logger::Log("SRVUAVManager Initialize\n");
}

SRVUAVManager::~SRVUAVManager() {
	Logger::Log("SRVUAVManager Finalize\n");
}

void SRVUAVManager::CreateDescriptorHeap() {
	// デスクリプタヒープの作成
	descriptorHeap_ = dxgi_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxViewCount_, true);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize_ = dxgi_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SRVUAVManager::CreateSrvTexture2d(uint32_t viewIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {
	// metaDataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;
	dxgi_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetDescriptorHandleCPU(viewIndex));
}

void SRVUAVManager::CreateSrvTextureCubeMap(uint32_t viewIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {
	// metaDataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension =D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MipLevels = UINT_MAX;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	dxgi_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetDescriptorHandleCPU(viewIndex));
}

void SRVUAVManager::CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;
	dxgi_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetDescriptorHandleCPU(viewIndex));
}

void SRVUAVManager::CreateSrvByteAddressBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t byteSize) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = byteSize / 4;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
	srvDesc.Buffer.StructureByteStride = 0;

	dxgi_->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetDescriptorHandleCPU(viewIndex));
}

void SRVUAVManager::CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = numElements;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = structureByteStride;
	dxgi_->GetDevice()->CreateUnorderedAccessView(pResource, nullptr, &uavDesc, GetDescriptorHandleCPU(viewIndex));
}
