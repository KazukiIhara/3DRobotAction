#include "RingDrawer3D.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "Const/Primitive3DConst.h"

// TODO::フレームワーク内にあるクラスはMAGIをインクルードしてはいけない
#include "Framework/MAGI.h"

using namespace MAGIMath;

RingDrawer3D::RingDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		instancingResource_[i] = dxgi_->CreateBufferResource(sizeof(RingData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		instancingSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex_[i], instancingResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(RingData3DForGPU));
		instancingResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_[i]));

		materialResource_[i] = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		materialSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_[i], materialResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
		materialResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&materialData_[i]));

		currentIndex_[i] = 0;
		instanceCount_[i] = 0;

		uint32_t texIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
		for (uint32_t j = 0; j < PrimitiveCommonConst::NumMaxInstance; ++j) {
			materialData_[i][j].textureIndex = texIndex;
			materialData_[i][j].baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			materialData_[i][j].uvMatrix = MakeIdentityMatrix4x4();
		}
	}

	Logger::Log("RingDrawer3D Initialize\n");
}

RingDrawer3D::~RingDrawer3D() {
	Logger::Log("RingDrawer3D Finalize\n");
}

void RingDrawer3D::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		assert(currentIndex_[i] <= PrimitiveCommonConst::NumMaxInstance);
		instanceCount_[i] = currentIndex_[i];
		currentIndex_[i] = 0;
	}
}

void RingDrawer3D::Draw(BlendMode mode) {
	const uint32_t i = static_cast<uint32_t>(mode);
	if (instanceCount_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Ring3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Ring3D, mode));
	camera3DManager_->TransferCurrentCamera(0);

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);

	commandList->DispatchMesh(1, instanceCount_[i], 1);
}

void RingDrawer3D::AddRing(const Matrix4x4& worldMatrix, const RingData3D& data, const MaterialData3D& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendMode);

#if defined(DEBUG) || defined(DEVELOP)
	if (currentIndex_[blendIndex] >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("RingDrawer3D: Max instance count exceeded!\n");
		return;
	}
#endif // _DEBUG

	// テクスチャ名を取得
	std::string textureName = material.textureName;

	// 設定されていなければデフォルトのテクスチャを設定
	if (textureName == "") {
		textureName = "EngineAssets/Images/uvChecker.png";
	}

	RingData3DForGPU newRingData{
		.worldMatrix = worldMatrix,
		.worldInverseTranspose = MakeInverseTransposeMatrix(worldMatrix),
		.ringDivide = data.ringDivide,
		.outerRadius = data.outerRadius,
		.innerRadius = data.innerRadius,
		.radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(data.ringDivide)
	};

	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = MAGISYSTEM::GetTextureIndex(textureName),
		.baseColor = material.baseColor,
		.uvMatrix = MakeUVMatrix(material.uvScale, material.uvRotate, material.uvTranslate)
	};

	instancingData_[blendIndex][currentIndex_[blendIndex]] = newRingData;
	materialData_[blendIndex][currentIndex_[blendIndex]] = newMaterialData;
	currentIndex_[blendIndex]++;
}

void RingDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void RingDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void RingDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void RingDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void RingDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}