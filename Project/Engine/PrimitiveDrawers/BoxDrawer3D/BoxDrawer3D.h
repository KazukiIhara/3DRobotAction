#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/Primitive3DStruct.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class ShadowPipelineManager;
class Camera3DManager;
class LightManager;

class BoxDrawer3D {
public:
	BoxDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		ShadowPipelineManager* shadowPipelineManager,
		Camera3DManager* camera3DManager,
		LightManager* lightManager
	);
	~BoxDrawer3D();

	void Update();
	void Draw(BlendMode mode);
	void DrawShadow(BlendMode mode);

	void AddBox(
		const Matrix4x4& worldMatrix,
		const BoxData3D& data,
		const PrimitiveMaterialData3D& material
	);

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);
	void SetLightManager(LightManager* lightManager);

private:
	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_[static_cast<uint32_t>(BlendMode::Num)];
	// instancing描画用のデータ
	BoxData3DForGPU* instancingData_[static_cast<uint32_t>(BlendMode::Num)];

	// マテリアルのリソース
	ComPtr<ID3D12Resource> materialResource_[static_cast<uint32_t>(BlendMode::Num)];
	// マテリアルデータ
	PrimitiveMaterialData3DForGPU* materialData_[static_cast<uint32_t>(BlendMode::Num)];

	// Plane3DSrvIndex
	uint32_t instancingSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];
	// MaterialSrvIndex
	uint32_t materialSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];

	// instance描画する際に使う変数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];

	// 現在のインデックス
	uint32_t currentIndex_[static_cast<uint32_t>(BlendMode::Num)];

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	ShadowPipelineManager* shadowPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
	LightManager* lightManager_ = nullptr;
};