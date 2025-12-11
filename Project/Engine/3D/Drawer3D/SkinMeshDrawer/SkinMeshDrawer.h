#pragma once

// DirectX
#include <d3d12.h>

// DirectXMesh
#include <DirectXMesh/DirectXMesh.h>

// MAGI
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/ModelStruct.h"
#include "Structs/Primitive3DStruct.h"

/// <summary>
/// スキンメッシュ描画クラス
/// </summary>
class SkinMeshDrawer {
public:
	SkinMeshDrawer(const MeshData& meshData);
	~SkinMeshDrawer() = default;

	void Skinning(const uint32_t& paletteSrvIndex);
	void Draw(uint32_t instanceCount);
	void DrawShadow(uint32_t instanceCount);
	// 影響度の参照を渡す
	std::span<VertexInfluenceJoints>& GetMappdInfluence();
private:
	void TransirionSkinResource(
		ID3D12GraphicsCommandList* cmd,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after
	);

private:
	// 頂点
	ComPtr<ID3D12Resource> vertexBuffer_;
	VertexData3D* vertexData_ = nullptr;
	uint32_t vertexCount_ = 0;
	uint32_t vertexSrvIdx_ = 0;

	// インデックスの数
	uint32_t indexCount_ = 0;

	// メッシュ分割
	ComPtr<ID3D12Resource> meshletBuffer_;              // StructuredBuffer<Meshlet>
	uint32_t meshletCount_ = 0;
	uint32_t meshletSrvIdx_ = 0;

	ComPtr<ID3D12Resource> meshletUniqueVertIB_;        // ByteAddressBuffer
	uint32_t uniqueVertSrvIdx_ = 0;

	ComPtr<ID3D12Resource> meshletPrimIB_;              // StructuredBuffer<MeshletTriangle>
	uint32_t primSrvIdx_ = 0;

	// メシュレットごとのバウンディングスフィアデータ
	ComPtr<ID3D12Resource> cullDataBuffer_;
	// メシュレットごとのバウンディングスフィア
	std::vector<DirectX::CullData> cullData_;
	uint32_t cullDataSrvIndex_ = 0;

	// マテリアル
	ComPtr<ID3D12Resource> materialBuffer_;
	ModelMaterialDataForGPU* material_ = nullptr;

	// スキニング後用頂点
	ComPtr<ID3D12Resource> skinnedVertexBuffer_;
	uint32_t skinnedVertexUavIdx_ = 0;
	uint32_t skinnedVertexSrvIdx_ = 0;
	D3D12_RESOURCE_STATES skinnedVertexResourceState_ = D3D12_RESOURCE_STATE_COMMON;

	// スキニング影響度のリソース
	ComPtr<ID3D12Resource> influenceResource_;
	// スキニング影響度
	std::span<VertexInfluenceJoints> mappedInfluence_;
	// スキニング影響度srvインデックス
	uint32_t influenceSrvIndex = 0;

	// スキニング用の情報リソース
	ComPtr<ID3D12Resource> skinningInformationResource_;
	// スキニング用の情報データ
	SkinningInformationForGPU* skiningInformationData_ = nullptr;
};