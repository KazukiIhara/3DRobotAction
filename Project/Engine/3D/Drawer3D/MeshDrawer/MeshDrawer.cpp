// MeshDrawer.cpp ― Mesh Shader + Meshlet （DXMesh v168 10-bit 版）
#define NOMINMAX
#include "MeshDrawer.h"

#include <cassert>
#include <cstring>
#include <DirectXMath.h>

#include "Framework/MAGI.h"
#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"
using namespace MAGIMath;
using namespace MAGIUtility;

// ─────────────────────────────────────────────
MeshDrawer::MeshDrawer(const MeshData& meshData) {
	/*=== 頂点 / インデックス ===================================================*/
	vertexCount_ = static_cast<uint32_t>(meshData.vertices.size());
	const std::string vertexCount = std::to_string(vertexCount_);
	indexCount_ = static_cast<uint32_t>(meshData.indices.size());
	assert(vertexCount_ && indexCount_);

	//=========================================================================
	//Meshlet
	//=========================================================================

	// 
	// モデルのデータ
	// 

	// インデックス配列
	std::vector<uint32_t> indices;
	// 頂点座標配列
	std::vector<DirectX::XMFLOAT3> positions;

	// 
	// モデルデータからメシュレット用のデータを作成
	// 

	// インデックスをコピー
	indices.resize(indexCount_);
	for (uint32_t i = 0; i < indexCount_; i++) {
		indices[i] = meshData.indices[i];
	}

	// 頂点データからポジションを取得
	positions.resize(vertexCount_);
	for (uint32_t i = 0; i < vertexCount_; i++) {
		positions[i] = DirectX::XMFLOAT3(
			meshData.vertices[i].position.x,
			meshData.vertices[i].position.y,
			meshData.vertices[i].position.z
		);
	}

	// 
	// 出力用データ
	// 

	// メシュレット
	std::vector<DirectX::Meshlet> meshlets;
	// ユニーク頂点インデックス
	std::vector<uint8_t> uniqueVertexIB;
	// 
	std::vector<DirectX::MeshletTriangle> primitiveIndices;

	// Meshlet計算
	HRESULT hr = DirectX::ComputeMeshlets(
		indices.data(), indices.size() / 3,
		positions.data(), positions.size(),
		nullptr,
		meshlets,
		uniqueVertexIB,
		primitiveIndices
	);
	assert(SUCCEEDED(hr));

	// メシュレットのサイズを取得
	meshletCount_ = static_cast<uint32_t>(meshlets.size());

	// メシュレットごとのバウンディングスフィア作成
	cullData_.resize(meshlets.size());

	hr = DirectX::ComputeCullData(positions.data(), positions.size(),
		meshlets.data(), meshlets.size(),
		reinterpret_cast<const uint32_t*>(uniqueVertexIB.data()),
		uniqueVertexIB.size() / sizeof(uint32_t),
		primitiveIndices.data(), primitiveIndices.size(),
		cullData_.data());

	assert(SUCCEEDED(hr));

	//=========================================================================
	//リソースの作成
	//=========================================================================

	// 頂点編
	vertexBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * vertexCount_);
	vertexSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(vertexSrvIdx_, vertexBuffer_.Get(), vertexCount_, sizeof(VertexData3D));

	// 頂点データをコピー
	{
		void* mappedPtr = nullptr;
		vertexBuffer_->Map(0, nullptr, &mappedPtr);
		std::memcpy(mappedPtr, meshData.vertices.data(), sizeof(VertexData3D) * vertexCount_);
		vertexBuffer_->Unmap(0, nullptr);
	}


	// メシュレット編
	meshletBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(DirectX::Meshlet) * static_cast<uint32_t>(meshlets.size()));
	meshletSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(
		meshletSrvIdx_,
		meshletBuffer_.Get(),
		static_cast<uint32_t>(meshlets.size()),
		sizeof(DirectX::Meshlet)
	);

	// バッファにデータ転送
	{
		void* mappedPtr = nullptr;
		meshletBuffer_->Map(0, nullptr, &mappedPtr);
		std::memcpy(mappedPtr, meshlets.data(), sizeof(DirectX::Meshlet) * meshlets.size());
		meshletBuffer_->Unmap(0, nullptr);
	}

	// ユニーク頂点インデックス編
	meshletUniqueVertIB_ = MAGISYSTEM::CreateBufferResource(static_cast<uint32_t>(uniqueVertexIB.size()));
	uniqueVertSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvByteAddressBuffer(
		uniqueVertSrvIdx_,
		meshletUniqueVertIB_.Get(),
		static_cast<uint32_t>(uniqueVertexIB.size())
	);

	// バッファにデータ転送
	{
		void* mappedPtr = nullptr;
		meshletUniqueVertIB_->Map(0, nullptr, &mappedPtr);
		std::memcpy(mappedPtr, uniqueVertexIB.data(), uniqueVertexIB.size());
		meshletUniqueVertIB_->Unmap(0, nullptr);
	}

	// PrimitiveIndices編
	meshletPrimIB_ = MAGISYSTEM::CreateBufferResource(sizeof(DirectX::MeshletTriangle) * static_cast<uint32_t>(primitiveIndices.size()));
	primSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(
		primSrvIdx_,
		meshletPrimIB_.Get(),
		static_cast<uint32_t>(primitiveIndices.size()),
		sizeof(DirectX::MeshletTriangle)
	);

	// バッファにデータ転送
	{
		void* mappedPtr = nullptr;
		meshletPrimIB_->Map(0, nullptr, &mappedPtr);
		std::memcpy(mappedPtr, primitiveIndices.data(), sizeof(DirectX::MeshletTriangle) * primitiveIndices.size());
		meshletPrimIB_->Unmap(0, nullptr);
	}

	// バウンディングスフィア
	cullDataBuffer_ = MAGISYSTEM::CreateBufferResource(
		sizeof(DirectX::CullData) * cullData_.size());
	cullDataSrvIndex_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(
		cullDataSrvIndex_, cullDataBuffer_.Get(),
		static_cast<uint32_t>(cullData_.size()),
		sizeof(DirectX::CullData)
	);

	{
		void* p = nullptr;
		cullDataBuffer_->Map(0, nullptr, &p);
		std::memcpy(p, cullData_.data(), sizeof(DirectX::CullData) * cullData_.size());
		cullDataBuffer_->Unmap(0, nullptr);
	}

	/*=== マテリアル ===========================================================*/
	materialBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(ModelMaterialDataForGPU));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
	*material_ = {
		.textureIndex = MAGISYSTEM::GetTextureIndex(meshData.material.textureFilePath),
		.baseColor = meshData.material.color,
		.uvMatrix = meshData.material.uvMatrix,
	};
	materialBuffer_->Unmap(0, nullptr);

}

MeshDrawer::~MeshDrawer() = default;

// -----------------------------------------------------------------------------
void MeshDrawer::Draw(uint32_t instanceCount) {
	if (!instanceCount) return;

	auto* cmd = MAGISYSTEM::GetDirectXCommandList6();

	cmd->SetGraphicsRootConstantBufferView(2, materialBuffer_->GetGPUVirtualAddress());
	cmd->SetGraphicsRootDescriptorTable(4, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexSrvIdx_));
	cmd->SetGraphicsRootDescriptorTable(5, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(meshletSrvIdx_));
	cmd->SetGraphicsRootShaderResourceView(6, meshletUniqueVertIB_->GetGPUVirtualAddress());
	cmd->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(primSrvIdx_));
	cmd->SetGraphicsRootDescriptorTable(8, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(cullDataSrvIndex_));
	MeshInfo info = {
		.indexSize = 4,
		.meshletCount = meshletCount_,
	};
	cmd->SetGraphicsRoot32BitConstants(9, 2, &info, 0);

	cmd->DispatchMesh(DivRoundUp(meshletCount_, AS_GROUP_SIZE), instanceCount, 1);
}

void MeshDrawer::DrawShadow(uint32_t instanceCount) {
	if (!instanceCount) return;

	auto* cmd = MAGISYSTEM::GetDirectXCommandList6();

	cmd->SetGraphicsRootDescriptorTable(2, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexSrvIdx_));
	cmd->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(meshletSrvIdx_));
	cmd->SetGraphicsRootShaderResourceView(4, meshletUniqueVertIB_->GetGPUVirtualAddress());
	cmd->SetGraphicsRootDescriptorTable(5, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(primSrvIdx_));
	cmd->SetGraphicsRootDescriptorTable(6, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(cullDataSrvIndex_));

	MeshInfo info = {
		.indexSize = 4,
		.meshletCount = meshletCount_,
	};
	cmd->SetGraphicsRoot32BitConstants(7, 2, &info, 0);

	cmd->DispatchMesh(DivRoundUp(meshletCount_, AS_GROUP_SIZE), instanceCount, 1);
}

void MeshDrawer::DrawBoundingSphere() {
	for (const auto& cd : cullData_) {
		const DirectX::XMFLOAT3 bs = cd.BoundingSphere.Center;
		const Vector3 center = { bs.x, bs.y, bs.z };
		const float   radius = cd.BoundingSphere.Radius;

		Matrix4x4 local = MakeTranslateMatrix(center);
		SphereData3D sphere{
			.radius = radius,
		};

		//MAGISYSTEM::DrawSphere3D(world, sphere, PrimitiveMaterialData3D{});
	}
}
