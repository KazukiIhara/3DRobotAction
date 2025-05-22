#pragma once

// C++
#include <vector>
#include <cstdint>

// DirectX
#include <d3d12.h>
#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/SkyBoxStruct.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

/// <summary>
/// 背景ボックス描画クラス
/// </summary>
class SkyBoxDrawer {
public:
	SkyBoxDrawer(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager
	);
	~SkyBoxDrawer();

	void Update();
	void Draw();

	void SetTextureIndex(uint32_t textureIndex);
	void TransferSkyBoxTexture(uint32_t rootParamIndex);

private:

	void CreateShape();

	void CreateVertexResource();
	void CreateVertexBufferView();
	void MapVertexData();

	void CreateIndexResource();
	void CreateIndexBufferView();
	void MapIndexData();

	void CreateSkyBoxResource();
	void MapSkyBoxData();

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);

private:
	// 頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// GPUに送る用の頂点データ
	SkyBoxVertexData3D* vertexData_ = nullptr;
	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	// 頂点データ
	std::vector<SkyBoxVertexData3D> vertices_;

	// インデックスリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// GPUに送る用のインデックスデータ
	uint32_t* indexData_ = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	// インデックスデータ
	std::vector<uint32_t> indices_;

	// スカイボックスのリソース
	ComPtr<ID3D12Resource> skyBoxResource_ = nullptr;
	// スカイボックスのデータ
	SkyBoxDataForGPU* skyBoxData_ = nullptr;


	// テクスチャのインデックス
	uint32_t textureIndex_ = 0;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;

};