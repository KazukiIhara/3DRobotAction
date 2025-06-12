#include "Mesh.h"

#include "Framework/MAGI.h"

Mesh::Mesh(const MeshData& meshData) {
	// メッシュのデータを受けとる
	meshData_ = meshData;
}

void Mesh::Initialize() {
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

	// マテリアル用のリソース作成
	CreateMaterialResource();
	// マテリアルにデータを書き込む
	MapMaterialData();

	// テクスチャが未設定の場合、デフォルトのテクスチャを割り当てる
	if (meshData_.material.textureFilePath == "") {
		meshData_.material.textureFilePath = "EngineAssets/Images/uvChecker.png";
	}

	// ノーマルマップがあるかチェック
	if (meshData_.material.normalMapTextureFilePath != "") {
		materialData_->enableNormalMap = true;
	} else {
		materialData_->enableNormalMap = false;
	}
}

void Mesh::Update() {
	// マテリアルを更新
	materialData_->color = meshData_.material.color;
	materialData_->uvMatrix = meshData_.material.uvMatrix;
}

void Mesh::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.textureFilePath].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrvIndex));

	// NormalMap用のSrvをセット
	if (meshData_.material.normalMapTextureFilePath != "") {
		uint32_t normalMapTextureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.normalMapTextureFilePath].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(normalMapTextureSrvIndex));
	} else {// 未定義動作を防ぐため、デフォルトのテクスチャのsrvIndexをセットしておく
		uint32_t defaultNormalMapTextureSrvIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(defaultNormalMapTextureSrvIndex));
	}

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(meshData_.indices.size()), 1, 0, 0, 0);
}

void Mesh::DrawInstanced(uint32_t instancedCount) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.textureFilePath].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrvIndex));

	// NormalMap用のSrvをセット
	if (meshData_.material.normalMapTextureFilePath != "") {
		uint32_t normalMapTextureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.normalMapTextureFilePath].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(normalMapTextureSrvIndex));
	} else {// 未定義動作を防ぐため、デフォルトのテクスチャのsrvIndexをセットしておく
		uint32_t defaultNormalMapTextureSrvIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(defaultNormalMapTextureSrvIndex));
	}

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(meshData_.indices.size()), instancedCount, 0, 0, 0);
}

void Mesh::DrawInstancedForParticle(uint32_t instancedCount) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.textureFilePath].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(2, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrvIndex));

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(meshData_.indices.size()), instancedCount, 0, 0, 0);
}

void Mesh::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * meshData_.vertices.size());
}

void Mesh::CreateVertexBufferView() {
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * meshData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}

void Mesh::MapVertexData() {
	// 頂点データ
	VertexData3D* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, meshData_.vertices.data(), sizeof(VertexData3D) * meshData_.vertices.size());
}

void Mesh::CreateIndexResource() {
	indexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * meshData_.indices.size());
}

void Mesh::CreateIndexBufferView() {
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * meshData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void Mesh::MapIndexData() {
	// インデックスデータ
	uint32_t* indexData = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	std::memcpy(indexData, meshData_.indices.data(), sizeof(uint32_t) * meshData_.indices.size());
}

void Mesh::CreateMaterialResource() {
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(MaterialForGPU));
}

void Mesh::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = meshData_.material.color;
	materialData_->uvMatrix = meshData_.material.uvMatrix;
}