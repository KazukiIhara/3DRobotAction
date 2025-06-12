#pragma once
// DirectX
#include <d3d12.h>

// DirectXMesh
#include <DirectXMesh/DirectXMesh.h>

// MAGI
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/ModelStruct.h"
#include "Structs/Primitive3DStruct.h"

/// <summary>メッシュ描画クラス（Mesh Shader + Meshlet）</summary>
class MeshDrawer {
public:
	MeshDrawer(const MeshData& meshData);
	~MeshDrawer();

	void Update();
	void Draw(uint32_t instanceCount);
	void DrawShadow(uint32_t instanceCount);

	void DrawBoundingSphere();

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
};
