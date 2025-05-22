#include "CylinderDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

CylinderDrawer3D::CylinderDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, ShadowPipelineManager* shadowPipelineManager, Camera3DManager* camera3DManager, LightManager* lightManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetShadowPipelineManager(shadowPipelineManager);
	SetCamera3DManager(camera3DManager);
	SetLightManager(lightManager);

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		instancingResource_[i] = dxgi_->CreateBufferResource(sizeof(CylinderData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		instancingSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex_[i], instancingResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(CylinderData3DForGPU));
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

	Logger::Log("CylinderDrawer3D Initialize\n");

}

CylinderDrawer3D::~CylinderDrawer3D() {
	Logger::Log("CylinderDrawer3D Finalize\n");
}

void CylinderDrawer3D::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		assert(currentIndex_[i] <= PrimitiveCommonConst::NumMaxInstance);
		instanceCount_[i] = currentIndex_[i];
		currentIndex_[i] = 0;
	}
}

void CylinderDrawer3D::Draw(BlendMode mode) {
	const uint32_t i = static_cast<uint32_t>(mode);
	if (instanceCount_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Cylinder3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Cylinder3D, mode));
	camera3DManager_->TransferCurrentCamera(0);

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);

	commandList->DispatchMesh(1, instanceCount_[i], 1);
}

void CylinderDrawer3D::DrawShadow(BlendMode mode) {
	const uint32_t i = static_cast<uint32_t>(mode);
	if (instanceCount_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(shadowPipelineManager_->GetRootSignature(ShadowPipelineStateType::Cylinder));
	commandList->SetPipelineState(shadowPipelineManager_->GetPipelineState(ShadowPipelineStateType::Cylinder));

	lightManager_->TransferDirectionalLightCamera(0);

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[i]));

	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(2, 1, &rootConstants, 0);

	commandList->DispatchMesh(1, instanceCount_[i], 1);
}

void CylinderDrawer3D::AddCylinder(const Matrix4x4& worldMatrix, const CylinderData3D& data, const PrimitiveMaterialData3D& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendMode);

#ifdef _DEBUG
	if (currentIndex_[blendIndex] >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("CylinderDrawer3D: Max instance count exceeded!\n");
		return;
	}
#endif // _DEBUG

	// テクスチャ名を取得
	std::string textureName = material.textureName;

	// 設定されていなければデフォルトのテクスチャを設定
	if (textureName == "") {
		textureName = "EngineAssets/Images/uvChecker.png";
	}

	CylinderData3DForGPU newCylinderData{
		.worldMatrix = worldMatrix,
		.worldInverseTranspose = MakeInverseTransposeMatrix(worldMatrix),
		.divide = data.divide,
		.topRadius = data.topRadius,
		.bottomRadiu = data.bottomRadius,
		.height = data.height
	};

	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = MAGISYSTEM::GetTextureIndex(textureName),
		.baseColor = material.baseColor,
		.uvMatrix = MakeUVMatrix(material.uvScale, material.uvRotate, material.uvTranslate)
	};

	instancingData_[blendIndex][currentIndex_[blendIndex]] = newCylinderData;
	materialData_[blendIndex][currentIndex_[blendIndex]] = newMaterialData;
	currentIndex_[blendIndex]++;
}

void CylinderDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void CylinderDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void CylinderDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void CylinderDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void CylinderDrawer3D::SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager) {
	assert(shadowPipelineManager);
	shadowPipelineManager_ = shadowPipelineManager;
}

void CylinderDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void CylinderDrawer3D::SetLightManager(LightManager* lightManager) {
	assert(lightManager);
	lightManager_ = lightManager;
}
