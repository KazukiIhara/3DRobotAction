#include "SpriteDrawer.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "Camera2DManager/Camera2DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include "DirectXTex/DirectXTex.h"

#include "Framework/MAGI.h"

#include <cassert>

using namespace MAGIUtility;
using namespace MAGIMath;

SpriteDrawer::SpriteDrawer(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	Camera2DManager* camera2DManager
) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera2DManager(camera2DManager);

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		// リソース作成

		// 
		// Front
		// 
		instancingResourceFront_[i] = dxgi_->CreateBufferResource(sizeof(SpriteDataForGPU) * NumMaxInstance);
		instancingSrvIndexFront_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndexFront_[i], instancingResourceFront_[i].Get(), NumMaxInstance, sizeof(SpriteDataForGPU));
		instancingResourceFront_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingDataFront_[i]));

		currentIndexFront_[i] = 0;
		instanceCountFront_[i] = 0;

		// 
		// Back
		// 
		instancingResourceBack_[i] = dxgi_->CreateBufferResource(sizeof(SpriteDataForGPU) * NumMaxInstance);
		instancingSrvIndexBack_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndexBack_[i], instancingResourceBack_[i].Get(), NumMaxInstance, sizeof(SpriteDataForGPU));
		instancingResourceBack_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingDataBack_[i]));

		currentIndexBack_[i] = 0;
		instanceCountBack_[i] = 0;
	}

	Logger::Log("SpriteDrawer Initialize\n");
}

SpriteDrawer::~SpriteDrawer() {
	Logger::Log("SpriteDrawer Finalize\n");
}

void SpriteDrawer::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		assert(currentIndexFront_[i] <= NumMaxInstance);
		instanceCountFront_[i] = currentIndexFront_[i];
		currentIndexFront_[i] = 0;

		assert(currentIndexBack_[i] <= NumMaxInstance);
		instanceCountBack_[i] = currentIndexBack_[i];
		currentIndexBack_[i] = 0;
	}
}

void SpriteDrawer::DrawFront(BlendMode blendMode) {
	const uint32_t i = static_cast<uint32_t>(blendMode);
	if (instanceCountFront_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Sprite));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Sprite, blendMode));

	camera2DManager_->TransferCurrentCamera(0);
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndexFront_[i]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(0));

	commandList->DispatchMesh(1, instanceCountFront_[i], 1);
}

void SpriteDrawer::DrawBack(BlendMode blendMode) {
	const uint32_t i = static_cast<uint32_t>(blendMode);
	if (instanceCountBack_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Sprite));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Sprite, blendMode));

	camera2DManager_->TransferCurrentCamera(0);
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndexBack_[i]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(0));

	commandList->DispatchMesh(1, instanceCountBack_[i], 1);
}

void SpriteDrawer::AddSprite(const SpriteData& data, const SpriteMaterialData& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendmode);
	if (data.isBack) {
		instancingDataBack_[blendIndex][currentIndexBack_[blendIndex]] = ComputeSpriteDataForGPU(data, material);
		currentIndexBack_[blendIndex]++;
	} else {
		instancingDataFront_[blendIndex][currentIndexFront_[blendIndex]] = ComputeSpriteDataForGPU(data, material);
		currentIndexFront_[blendIndex]++;
	}
}

SpriteDataForGPU SpriteDrawer::ComputeSpriteDataForGPU(const SpriteData& data, const SpriteMaterialData& material) {
	// テクスチャ名を取得
	std::string textureName = material.textureName;

	// 設定されていなければデフォルトのテクスチャを設定
	if (textureName == "") {
		textureName = "EngineAssets/Images/uvChecker.png";
	}

	// テクスチャメタデータ取得
	const DirectX::TexMetadata& metaData = MAGISYSTEM::GetTextureMetaData(textureName);

	// アンカーポイントの設定
	float left = 0.0f - material.anchorPoint.x;
	float right = 1.0f - material.anchorPoint.x;
	float top = 0.0f - material.anchorPoint.y;
	float bottom = 1.0f - material.anchorPoint.y;

	// 切り取りサイズ
	Vector2 cutOutSize = material.textureCutOutSize;
	// 切り取りサイズが設定されていない場合はmetaDataから取得
	if (cutOutSize.x == -1.0f) {
		cutOutSize.x = static_cast<float>(metaData.width);
	}

	if (cutOutSize.y == -1.0f) {
		cutOutSize.y = static_cast<float>(metaData.height);
	}

	// スプライトのサイズ
	Vector2 size = data.size;
	// サイズが設定されていない場合、切り取りサイズと同じにする
	if (size.x == -1.0f) {
		size.x = cutOutSize.x;
	}

	if (size.y == -1.0f) {
		size.y = cutOutSize.y;
	}

	float texLeft = material.textureLeftTop.x / metaData.width;
	float texRight = (material.textureLeftTop.x + cutOutSize.x) / metaData.width;
	float texTop = material.textureLeftTop.y / metaData.height;
	float texBottom = (material.textureLeftTop.y + cutOutSize.y) / metaData.height;

	// 左右反転
	if (material.isFlipX) {
		texLeft = -texLeft;
		texRight = -texRight;
	}
	// 上下反転
	if (material.isFlipY) {
		texTop = -texTop;
		texBottom = -texBottom;
	}

	// 前景か背景か
	float z = data.isBack ? 10000.0f : 0.0f;
	// 追加するスプライトデータ
	SpriteDataForGPU newSpriteData{
		.worldMatrix = MakeAffineMatrix(Vector3(size.x,size.y,1.0f),Vector3(0.0f,0.0f,data.rotate),Vector3(data.position.x,data.position.y,z)),
		.vertexPosition{
			{left,top},			// 左上
			{right,top},		// 右上
			{left,bottom},		// 左下
			{right,bottom},		// 右下
		},
		.texcooed{
			{texLeft,texTop},
			{texRight,texTop},
			{texLeft,texBottom},
			{texRight,texBottom},
		},
		.textureIndex = MAGISYSTEM::GetTextureIndex(textureName),
		.baseColor = material.color,
		.uvMatrix = MakeUVMatrix(material.uvScale,material.uvRotate,material.uvTranslate),
	};

	return newSpriteData;
}

void SpriteDrawer::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SpriteDrawer::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void SpriteDrawer::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void SpriteDrawer::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void SpriteDrawer::SetCamera2DManager(Camera2DManager* camera2DManager) {
	assert(camera2DManager);
	camera2DManager_ = camera2DManager;
}
