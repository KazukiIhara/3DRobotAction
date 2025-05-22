#pragma once

// C++
#include <string>
#include <cstdint>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/ModelStruct.h"
#include "Structs/SkinningStruct.h"
#include "Structs/SkeletonStruct.h"

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh {
public:
	Mesh(const MeshData& meshData);
	virtual ~Mesh() = default;
	virtual void Initialize();
	void Update();
	virtual void Draw();
	virtual void DrawInstanced(uint32_t instancedCount);
	virtual void DrawInstancedForParticle(uint32_t instancedCount);

protected:
	// 頂点リソースの作成
	virtual void CreateVertexResource();
	// 頂点バッファービューの作成
	void CreateVertexBufferView();
	// 頂点データの書き込み
	void MapVertexData();

	// インデックスリソースの作成
	void CreateIndexResource();
	// インデックスバッファビューの作成
	void CreateIndexBufferView();
	// インデックスデータの書き込み
	void MapIndexData();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();

protected:
	// メッシュデータ
	MeshData meshData_{};

	// 頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// インデックスリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// IBV
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	MaterialForGPU* materialData_ = nullptr;
};