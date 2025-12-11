#include "TextureDataContainer.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/Fence/Fence.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

TextureDataContainer::TextureDataContainer(DXGI* dxgi, DirectXCommand* directXCommand, Fence* fence, SRVUAVManager* srvUavManager) {
	Initialize(dxgi, directXCommand, fence, srvUavManager);
	Logger::Log("TextureDataContainer Initialize\n");
}

TextureDataContainer::~TextureDataContainer() {
	Logger::Log("TextureDataContainer Finalize\n");
}

void TextureDataContainer::Initialize(DXGI* dxgi, DirectXCommand* directXCommand, Fence* fence, SRVUAVManager* srvUavManager) {
	// 必要なインスタンスのポインタ
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetFence(fence);
	SetSrvUavManager(srvUavManager);

	// デフォルトのテクスチャをロード
	defaultTextureIndex_ = Load("EngineAssets/Images/uvChecker.png");

}

uint32_t TextureDataContainer::Load(const std::string& fileName, bool isFullPath) {
	// テクスチャがすでに読み込まれているかチェック
	auto it = textureDatas_.find(fileName);
	if (it != textureDatas_.end()) {
		// すでに読み込まれている場合、早期リターン
		return it->second.srvIndex;
	}

	// 今回ぶち込むテクスチャーの箱
	Texture& texture = textureDatas_[fileName];
	DirectX::ScratchImage mipImage;

	if (isFullPath) {
		mipImage = LoadTexture(fileName);
	} else {
		// フルパス作成
		const std::string textureDirectoryFilePath = "Assets/Images/";
		mipImage = LoadTexture(textureDirectoryFilePath + fileName);
	}

	texture.metaData = mipImage.GetMetadata();
	texture.resource = CreateTextureResource(texture.metaData);
	ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(texture.resource.Get(), mipImage);

	// コマンドのクローズと実行
	directXCommand_->KickCommand();
	fence_->WaitGPU();
	directXCommand_->ResetCommand();

	// SRVを作成するDescriptorHeapの場所を決める
	texture.srvIndex = srvUavManager_->Allocate();

	if (texture.metaData.IsCubemap()) {
		// CubeMap用のsrvの作成
		srvUavManager_->CreateSrvTextureCubeMap(texture.srvIndex, textureDatas_[fileName].resource.Get(), texture.metaData.format);
	} else {
		// 通常テクスチャのsrvの作成
		srvUavManager_->CreateSrvTexture2d(texture.srvIndex, textureDatas_[fileName].resource.Get(), texture.metaData.format, UINT(texture.metaData.mipLevels));
	}

	// テクスチャ枚数上限チェック
	assert(srvUavManager_->IsLowerViewMax());

	return texture.srvIndex;
}

void TextureDataContainer::LoadNormalMap(const std::string& filePath) {
	// テクスチャがすでに読み込まれているかチェック
	auto it = textureDatas_.find(filePath);
	if (it != textureDatas_.end()) {
		// すでに読み込まれている場合、早期リターン
		return;
	}

	// 今回ぶち込むテクスチャーの箱
	Texture& texture = textureDatas_[filePath];
	DirectX::ScratchImage mipImage;

	mipImage = LoadNormalMapTexture(filePath);

	texture.metaData = mipImage.GetMetadata();
	texture.resource = CreateTextureResource(texture.metaData);
	ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(texture.resource.Get(), mipImage);

	// コマンドのクローズと実行
	directXCommand_->KickCommand();
	fence_->WaitGPU();
	directXCommand_->ResetCommand();

	// SRVを作成するDescriptorHeapの場所を決める
	texture.srvIndex = srvUavManager_->Allocate();
	// srvの作成
	srvUavManager_->CreateSrvTexture2d(texture.srvIndex, textureDatas_[filePath].resource.Get(), texture.metaData.format, UINT(texture.metaData.mipLevels));

	// テクスチャ枚数上限チェック
	assert(srvUavManager_->IsLowerViewMax());
}

std::unordered_map<std::string, Texture>& TextureDataContainer::GetTexture() {
	return textureDatas_;
}

const DirectX::TexMetadata& TextureDataContainer::GetMetaData(const std::string& filePath) {
	assert(&GetTexture()[filePath]);
	Texture& texture = GetTexture()[filePath];
	return texture.metaData;
}

const std::unordered_map<std::string, Texture>& TextureDataContainer::GetTextureContainer()const {
	return textureDatas_;
}

uint32_t TextureDataContainer::GetDefaultTextureIndex() const {
	return defaultTextureIndex_;
}

DirectX::ScratchImage TextureDataContainer::LoadTexture(const std::string& filePath) {
	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = Logger::ConvertString(filePath);

	HRESULT hr{};

	if (filePathW.ends_with(L".dds")) {
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	} else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}
	assert(SUCCEEDED(hr));

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		mipImages = std::move(image);
	} else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	}
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

DirectX::ScratchImage TextureDataContainer::LoadNormalMapTexture(const std::string& filePath) {
	// テクスチャファイルをWICで読む
	DirectX::ScratchImage image{};
	std::wstring filePathW = Logger::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(
		filePathW.c_str(),
		DirectX::WIC_FLAGS_IGNORE_SRGB, // ガンマ補正しない
		nullptr,
		image
	);
	assert(SUCCEEDED(hr));

	// 2) ミップマップを作る
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT,
		0,
		mipImages
	);
	assert(SUCCEEDED(hr));

	return mipImages;
}

ComPtr<ID3D12Resource> TextureDataContainer::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);	// Textureの幅
	resourceDesc.Height = UINT(metadata.height);// Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);// mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);// 奥行きor配列Textureの配列数
	resourceDesc.Format = metadata.format;// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;// サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//textureの次元数。普段使っているのは2次元

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = dxgi_->GetDevice()->CreateCommittedResource(
		&heapProperties,// Heapの設定
		D3D12_HEAP_FLAG_NONE,// Heapの特殊な設定。特になし。
		&resourceDesc,// リソースの設定
		D3D12_RESOURCE_STATE_COPY_DEST,//データ転送される設定
		nullptr,// Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));// 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

ComPtr<ID3D12Resource> TextureDataContainer::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(dxgi_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ComPtr<ID3D12Resource> intermediateResource = dxgi_->CreateBufferResource(intermediateSize);
	UpdateSubresources(directXCommand_->GetList(), texture, intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	// Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	directXCommand_->GetList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

void TextureDataContainer::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void TextureDataContainer::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void TextureDataContainer::SetFence(Fence* fence) {
	assert(fence);
	fence_ = fence;
}

void TextureDataContainer::SetSrvUavManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}
