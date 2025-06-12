#pragma once

// C++
#include <string>
#include <cstdint>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/ModelStruct.h"
#include "Enums/Primitive3DEnum.h"

/// <summary>
/// 3Dプリミティブ形状の基底クラス
/// </summary>
class BasePrimitiveShape3D {
public:
	BasePrimitiveShape3D();
	virtual ~BasePrimitiveShape3D() = default;
	void Initialize();
	virtual void SetShape() = 0;
	virtual void Update();
	void Draw();
	void DrawInstanced(uint32_t instanceCount);

	void SetIsNormalMap(bool enableNormalMap);

private:
	// 頂点リソースの作成
	void CreateVertexResource();
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
	// 形状データ
	PrimitiveData primitiveData_{};
private:

	// 頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// 頂点データ
	VertexData3D* vertexData_ = nullptr;
	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// インデックスリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// インデックスデータ
	uint32_t* indexData_ = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	MaterialForGPU* materialData_ = nullptr;
};