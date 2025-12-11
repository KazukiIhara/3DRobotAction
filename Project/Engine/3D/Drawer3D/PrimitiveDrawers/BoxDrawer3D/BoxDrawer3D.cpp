#include "BoxDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "PipelineManagers/ShadowPipelineManager/ShadowPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"
#include "LightManager/LightManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIUtility;
using namespace MAGIMath;

BoxDrawer3D::BoxDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, ShadowPipelineManager* shadowPipelineManager, Camera3DManager* camera3DManager, LightManager* lightManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetShadowPipelineManager(shadowPipelineManager);
	SetCamera3DManager(camera3DManager);
	SetLightManager(lightManager);
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		// リソース作成
		instancingResource_[i] = dxgi_->CreateBufferResource(sizeof(BoxData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		instancingSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex_[i], instancingResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(BoxData3DForGPU));
		instancingResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_[i]));

		materialResource_[i] = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		materialSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_[i], materialResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
		materialResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&materialData_[i]));

		currentIndex_[i] = 0;
		instanceCount_[i] = 0;

	}
	Logger::Log("BoxDrawer3D Initialize\n");
}

BoxDrawer3D::~BoxDrawer3D() {
	Logger::Log("BoxDrawer3D Finalize\n");
}

void BoxDrawer3D::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		assert(currentIndex_[i] <= PrimitiveCommonConst::NumMaxInstance);
		instanceCount_[i] = currentIndex_[i];
		currentIndex_[i] = 0;
	}
}

void BoxDrawer3D::Draw(BlendMode mode) {
	const uint32_t i = static_cast<uint32_t>(mode);
	if (instanceCount_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Box3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Box3D, mode));

	camera3DManager_->TransferCurrentCamera(0); // b0

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0)); // t1000

	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);

	commandList->DispatchMesh(1, instanceCount_[i], 1);
}

void BoxDrawer3D::DrawShadow(BlendMode mode) {
	const uint32_t i = static_cast<uint32_t>(mode);
	if (instanceCount_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(shadowPipelineManager_->GetRootSignature(ShadowPipelineStateType::Box));
	commandList->SetPipelineState(shadowPipelineManager_->GetPipelineState(ShadowPipelineStateType::Box));

	lightManager_->TransferDirectionalLightCamera(0);

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[i]));

	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(2, 1, &rootConstants, 0);

	commandList->DispatchMesh(1, instanceCount_[i], 1);
}

void BoxDrawer3D::AddBox(const Matrix4x4& worldMatrix, const BoxData3D& data, const MaterialData3D& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendMode);

#if defined(DEBUG) || defined(DEVELOP)
	if (currentIndex_[blendIndex] >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("BoxDrawer3D: Max instance count exceeded!\n");
		return;
	}
#endif // _DEBUG

	// テクスチャ名を取得
	std::string textureName = material.textureName;

	// 設定されていなければデフォルトのテクスチャを設定
	if (textureName.empty()) {
		textureName = "EngineAssets/Images/uvChecker.png";
	}

	// 書き込み先を持ってくる
	BoxData3DForGPU& writeBoxData = instancingData_[blendIndex][currentIndex_[blendIndex]];
	PrimitiveMaterialData3DForGPU& writeMatData = materialData_[blendIndex][currentIndex_[blendIndex]];

	writeBoxData = BoxData3DForGPU{
	  .worldMatrix = worldMatrix,
	  .worldInverseTranspose = MakeInverseTransposeMatrix(worldMatrix),
	  .offsets = {
		  Vector4(data.verticesOffsets[0].x, data.verticesOffsets[0].y, data.verticesOffsets[0].z, 1.0f),
		  Vector4(data.verticesOffsets[1].x, data.verticesOffsets[1].y, data.verticesOffsets[1].z, 1.0f),
		  Vector4(data.verticesOffsets[2].x, data.verticesOffsets[2].y, data.verticesOffsets[2].z, 1.0f),
		  Vector4(data.verticesOffsets[3].x, data.verticesOffsets[3].y, data.verticesOffsets[3].z, 1.0f),

		  Vector4(data.verticesOffsets[4].x, data.verticesOffsets[4].y, data.verticesOffsets[4].z, 1.0f),
		  Vector4(data.verticesOffsets[5].x, data.verticesOffsets[5].y, data.verticesOffsets[5].z, 1.0f),
		  Vector4(data.verticesOffsets[6].x, data.verticesOffsets[6].y, data.verticesOffsets[6].z, 1.0f),
		  Vector4(data.verticesOffsets[7].x, data.verticesOffsets[7].y, data.verticesOffsets[7].z, 1.0f),
	  }
	};

	writeMatData = PrimitiveMaterialData3DForGPU{
	   .textureIndex = MAGISYSTEM::GetTextureIndex(textureName),
	   .baseColor = material.baseColor,
	   .uvMatrix = MakeUVMatrix(material.uvScale, material.uvRotate, material.uvTranslate),
	};

	currentIndex_[blendIndex]++;
}

void BoxDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void BoxDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void BoxDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void BoxDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void BoxDrawer3D::SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager) {
	assert(shadowPipelineManager);
	shadowPipelineManager_ = shadowPipelineManager;
}

void BoxDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void BoxDrawer3D::SetLightManager(LightManager* lightManager) {
	assert(lightManager);
	lightManager_ = lightManager;
}
