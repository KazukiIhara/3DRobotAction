#include "SkyBoxDrawer.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Framework/MAGI.h"

#include <cassert>

using namespace MAGIUtility;
using namespace MAGIMath;

SkyBoxDrawer::SkyBoxDrawer(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	Camera3DManager* camera3DManager
) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);

	// 形状を生成
	CreateShape();

	// 頂点リソースの作成
	CreateVertexResource();
	// 頂点バッファビューの作成
	CreateVertexBufferView();
	// 頂点データの書き込み
	MapVertexData();

	// 描画用のインデックスリソースを作成
	CreateIndexResource();
	// インデックスバッファビューの作成
	CreateIndexBufferView();
	// インデックスリソースにデータを書き込む
	MapIndexData();

	// スカイボックス用のリソースを作成
	CreateSkyBoxResource();
	// スカイボックス用のデータをマップ
	MapSkyBoxData();

	textureIndex_ = MAGISYSTEM::LoadTexture("rostock_laage_airport_4k.dds");

	// 開始ログを出力
	Logger::Log("SkyBoxDrawer Initialize\n");
}

SkyBoxDrawer::~SkyBoxDrawer() {
	// 終了ログを出力
	Logger::Log("SkyBoxDrawer Finalize\n");
}

void SkyBoxDrawer::Update() {
	Camera3D* currentCamera = camera3DManager_->GetCurrentCamera();
	Vector3 translate = currentCamera->GetEye();
	float farClipRange = currentCamera->GetFarClipRange() * 0.9f;
	Vector3 scale = { farClipRange,farClipRange,farClipRange };

	Matrix4x4 scaleMat = MakeScaleMatrix(scale);
	Matrix4x4 translateMat = MakeTranslateMatrix(translate);

	skyBoxData_->worldMatrix = scaleMat * translateMat;
}

void SkyBoxDrawer::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// パイプラインをセット
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::SkyBox));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::SkyBox, BlendMode::None));

	// 形状設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// スカイボックスのデータをセット
	commandList->SetGraphicsRootConstantBufferView(0, skyBoxResource_->GetGPUVirtualAddress());

	// カメラをセット
	camera3DManager_->TransferCurrentCamera(1);

	// Texture用のSRVをセット
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(textureIndex_));

	// 描画
	commandList->DrawIndexedInstanced(UINT(indices_.size()), 1, 0, 0, 0);
}

void SkyBoxDrawer::SetTextureIndex(uint32_t textureIndex) {
	textureIndex_ = textureIndex;
}

void SkyBoxDrawer::TransferSkyBoxTexture(uint32_t rootParamIndex) {
	directXCommand_->GetList()->SetGraphicsRootDescriptorTable(rootParamIndex, srvUavManager_->GetDescriptorHandleGPU(textureIndex_));
}

void SkyBoxDrawer::CreateShape() {
	// 形状を作成
	vertices_.resize(24);
	indices_.resize(36);

	// 右面
	vertices_[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertices_[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertices_[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertices_[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
	// 左面
	vertices_[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertices_[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertices_[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertices_[7].position = { -1.0f,-1.0f,1.0f,1.0f };
	// 前面
	vertices_[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertices_[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertices_[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertices_[11].position = { 1.0f,-1.0f,1.0f,1.0f };
	// 上面
	vertices_[12].position = { -1.0f,1.0f,1.0f,1.0f };
	vertices_[13].position = { 1.0f,1.0f,1.0f,1.0f };
	vertices_[14].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertices_[15].position = { 1.0f,1.0f,-1.0f,1.0f };
	// 下面
	vertices_[16].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertices_[17].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertices_[18].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertices_[19].position = { 1.0f,-1.0f,-1.0f,1.0f };
	// 背面
	vertices_[20].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertices_[21].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertices_[22].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertices_[23].position = { -1.0f,-1.0f,-1.0f,1.0f };

	indices_ = {
		// 右面
		0, 1, 2, 2, 1, 3,
		// 左面
		4, 5, 6, 6, 5, 7,
		// 前面
		8, 9, 10, 10, 9, 11,
		// 上面
		12, 14, 13, 13, 14, 15,
		// 下面
		16, 17, 18, 18, 17, 19,
		// 背面
		20, 21, 22, 22, 21, 23
	};

}

void SkyBoxDrawer::CreateVertexResource() {
	vertexResource_ = dxgi_->CreateBufferResource(sizeof(SkyBoxVertexData3D) * vertices_.size());
}

void SkyBoxDrawer::CreateVertexBufferView() {
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(SkyBoxVertexData3D) * vertices_.size());
	vertexBufferView_.StrideInBytes = sizeof(SkyBoxVertexData3D);
}

void SkyBoxDrawer::MapVertexData() {
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, vertices_.data(), sizeof(SkyBoxVertexData3D) * vertices_.size());
}

void SkyBoxDrawer::CreateIndexResource() {
	indexResource_ = dxgi_->CreateBufferResource(sizeof(uint32_t) * indices_.size());
}

void SkyBoxDrawer::CreateIndexBufferView() {
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indices_.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void SkyBoxDrawer::MapIndexData() {
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, indices_.data(), sizeof(uint32_t) * indices_.size());
}

void SkyBoxDrawer::CreateSkyBoxResource() {
	skyBoxResource_ = dxgi_->CreateBufferResource(sizeof(SkyBoxDataForGPU));
}

void SkyBoxDrawer::MapSkyBoxData() {
	skyBoxData_ = nullptr;
	skyBoxResource_->Map(0, nullptr, reinterpret_cast<void**>(&skyBoxData_));
	skyBoxData_->worldMatrix = MakeIdentityMatrix4x4();
}

void SkyBoxDrawer::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SkyBoxDrawer::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void SkyBoxDrawer::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void SkyBoxDrawer::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void SkyBoxDrawer::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}
