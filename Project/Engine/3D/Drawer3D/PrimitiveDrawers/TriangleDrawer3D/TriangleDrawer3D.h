#pragma once

#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/Primitive3DStruct.h"
#include "Enums/BlendModeEnum.h"
#include "Const/Primitive3DConst.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

class TriangleDrawer3D {
public:
	TriangleDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager
	);
	~TriangleDrawer3D();

	void Update();
	void Draw(BlendMode mode);

	void AddTriangle(
		const Matrix4x4& worldMatrix,
		const TriangleData3D& data,
		const MaterialData3D& material
	);

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);

private:
	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_[static_cast<uint32_t>(BlendMode::Num)];
	// instancing描画用のデータ
	TriangleData3DForGPU* instancingData_[static_cast<uint32_t>(BlendMode::Num)];

	// マテリアルのリソース
	ComPtr<ID3D12Resource> materialResource_[static_cast<uint32_t>(BlendMode::Num)];
	// マテリアルデータ
	PrimitiveMaterialData3DForGPU* materialData_[static_cast<uint32_t>(BlendMode::Num)];

	// Triangle3DSrvIndex
	uint32_t instancingSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];
	// MaterialSrvIndex
	uint32_t materialSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];

	// instance描画する際に使う変数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];

	// 三角形追加時のインデックス
	uint32_t currentIndex_[static_cast<uint32_t>(BlendMode::Num)];

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;

};