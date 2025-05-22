#include "BasePrimitiveShape3D.h"

#include "Framework/MAGI.h"

BasePrimitiveShape3D::BasePrimitiveShape3D() {
}

void BasePrimitiveShape3D::Initialize() {
	// 形状を取得
	SetShape();

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
}

void BasePrimitiveShape3D::Update() {
	// マテリアルを更新
	materialData_->color = primitiveData_.color;
	materialData_->uvMatrix = primitiveData_.uvMatrix;
	materialData_->enableNormalMap = primitiveData_.enableNormalMap;
}

void BasePrimitiveShape3D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画
	commandList->DrawIndexedInstanced(UINT(primitiveData_.indices.size()), 1, 0, 0, 0);
}

void BasePrimitiveShape3D::DrawInstanced(uint32_t instanceCount) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, materialResource_->GetGPUVirtualAddress());
	// 描画
	commandList->DrawIndexedInstanced(UINT(primitiveData_.indices.size()), instanceCount, 0, 0, 0);
}

void BasePrimitiveShape3D::SetIsNormalMap(bool enableNormalMap) {
	primitiveData_.enableNormalMap = enableNormalMap;
}

void BasePrimitiveShape3D::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * primitiveData_.vertices.size());
}

void BasePrimitiveShape3D::CreateVertexBufferView() {
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * primitiveData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}

void BasePrimitiveShape3D::MapVertexData() {
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, primitiveData_.vertices.data(), sizeof(VertexData3D) * primitiveData_.vertices.size());
}

void BasePrimitiveShape3D::CreateIndexResource() {
	indexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * primitiveData_.indices.size());
}

void BasePrimitiveShape3D::CreateIndexBufferView() {
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * primitiveData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void BasePrimitiveShape3D::MapIndexData() {
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, primitiveData_.indices.data(), sizeof(uint32_t) * primitiveData_.indices.size());
}

void BasePrimitiveShape3D::CreateMaterialResource() {
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(MaterialForGPU));
}

void BasePrimitiveShape3D::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = primitiveData_.color;
	materialData_->uvMatrix = primitiveData_.uvMatrix;
}
