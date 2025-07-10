#include "SkinningMesh.h"

#include "Framework/MAGI.h"

SkinningMesh::SkinningMesh(const MeshData& meshData)
	:Mesh(meshData) {

}

void SkinningMesh::Initialize() {
	// 基底クラスの初期化
	Mesh::Initialize();

	// スキニング影響度のリソース作成
	CreateInfluenceResource();

	// スキニング用の頂点リソースを作成
	CreateSkinningVertexResources();
	// スキニング用の頂点バッファビューを作成
	CreateSkinningVertexBufferView();
	// スキニング用情報のリソースを作成
	CreateSkinningInformationResource();
	// スキニング情報用のデータを書き込み
	MapSkinningInformationData();

}

void SkinningMesh::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &skinningVertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.textureFilePath].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrvIndex));

	// NormalMap用のSrvをセット
	if (meshData_.material.normalMapTextureFilePath != "") {
		uint32_t normalMapTextureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.normalMapTextureFilePath].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(normalMapTextureSrvIndex));
	}

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(meshData_.indices.size()), 1, 0, 0, 0);

}

void SkinningMesh::DrawInstancedForParticle(uint32_t instancedCount) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &skinningVertexBufferView_);
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

void SkinningMesh::Skinning(const uint32_t& paletteSrvIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();

	// パイプライン設定
	commandList->SetComputeRootSignature(MAGISYSTEM::GetComputeRootSignature(ComputePipelineStateType::Skinning));
	commandList->SetPipelineState(MAGISYSTEM::GetComputePipelineState(ComputePipelineStateType::Skinning));

	// DescriptoorHeapの設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { MAGISYSTEM::GetSrvUavDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// コマンドを積む
	commandList->SetComputeRootDescriptorTable(0, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(paletteSrvIndex));
	commandList->SetComputeRootDescriptorTable(1, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexSrvIndex_));
	commandList->SetComputeRootDescriptorTable(2, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(influenceSrvIndex));
	commandList->SetComputeRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexUavIndex_));
	commandList->SetComputeRootConstantBufferView(4, skinningInformationResource_->GetGPUVirtualAddress());

	// コマンド発行
	commandList->Dispatch(UINT(meshData_.vertices.size() + 1023) / 1024, 1, 1);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = skinningVertexResource_.Get();
	commandList->ResourceBarrier(1, &barrier);

	// コマンド実行
	MAGISYSTEM::KickCommand();
	MAGISYSTEM::WaitGPU();
	MAGISYSTEM::ResetCommand();
}

std::span<VertexInfluenceJoints>& SkinningMesh::GetMappdInfluence() {
	return mappedInfluence_;
}

void SkinningMesh::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * meshData_.vertices.size());
	vertexSrvIndex_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(vertexSrvIndex_, vertexResource_.Get(), static_cast<uint32_t>(meshData_.vertices.size()), sizeof(VertexData3D));
}

void SkinningMesh::CreateSkinningVertexResources() {
	skinningVertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * meshData_.vertices.size(), true);
	vertexUavIndex_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateUavStructuredBuffer(vertexUavIndex_, skinningVertexResource_.Get(), static_cast<uint32_t>(meshData_.vertices.size()), sizeof(VertexData3D));
}

void SkinningMesh::CreateSkinningVertexBufferView() {
	skinningVertexBufferView_.BufferLocation = skinningVertexResource_->GetGPUVirtualAddress();
	skinningVertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * meshData_.vertices.size());
	skinningVertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}

void SkinningMesh::CreateSkinningInformationResource() {
	skinningInformationResource_ = nullptr;
	skinningInformationResource_ = MAGISYSTEM::CreateBufferResource(sizeof(SkinningInformationForGPU));
}

void SkinningMesh::MapSkinningInformationData() {
	skiningInformationData_ = nullptr;
	skinningInformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&skiningInformationData_));
	skiningInformationData_->numVertices = static_cast<uint32_t>(meshData_.vertices.size());
}

void SkinningMesh::CreateInfluenceResource() {
	// リソースを確保
	influenceResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexInfluenceJoints) * meshData_.vertices.size());
	VertexInfluenceJoints* mappedInfluence = nullptr;
	influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluenceJoints) * meshData_.vertices.size());
	mappedInfluence_ = { mappedInfluence, meshData_.vertices.size() };
	// インデックス割り当て
	influenceSrvIndex = MAGISYSTEM::SrvUavAllocate();
	// srv作成
	MAGISYSTEM::CreateSrvStructuredBuffer(influenceSrvIndex, influenceResource_.Get(), static_cast<uint32_t>(meshData_.vertices.size()), sizeof(VertexInfluenceJoints));
}
