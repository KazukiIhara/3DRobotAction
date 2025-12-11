#include "TriangleDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIUtility;
using namespace MAGIMath;

TriangleDrawer3D::TriangleDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		// リソース作成
		instancingResource_[i] = dxgi_->CreateBufferResource(sizeof(TriangleData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		instancingSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex_[i], instancingResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(TriangleData3DForGPU));
		instancingResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_[i]));

		materialResource_[i] = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		materialSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_[i], materialResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
		materialResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&materialData_[i]));

		currentIndex_[i] = 0;
		instanceCount_[i] = 0;

		// デフォルトマテリアル
		uint32_t texIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
		for (uint32_t j = 0; j < PrimitiveCommonConst::NumMaxInstance; ++j) {
			materialData_[i][j].textureIndex = texIndex;
			materialData_[i][j].baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			materialData_[i][j].uvMatrix = MakeIdentityMatrix4x4();
		}
	}

	Logger::Log("TriangleDrawer3D Initialize\n");
}

TriangleDrawer3D::~TriangleDrawer3D() {
	Logger::Log("TriangleDrawer3D Finalize\n");
}

void TriangleDrawer3D::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		assert(currentIndex_[i] <= PrimitiveCommonConst::NumMaxInstance);
		instanceCount_[i] = currentIndex_[i];
		currentIndex_[i] = 0;
	}
}

void TriangleDrawer3D::Draw(BlendMode mode) {
	const uint32_t blendIndex = static_cast<uint32_t>(mode);
	if (instanceCount_[blendIndex] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	// PSOとRootSignature設定
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Triangle3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Triangle3D, mode));

	// カメラ転送（b0）
	camera3DManager_->TransferCurrentCamera(0);

	// StructuredBuffer設定（t0, t1）
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[blendIndex]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_[blendIndex]));

	// Bindless Texture（t1000）
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	// RootConstants（b1）
	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);


	// 描画実行
	commandList->DispatchMesh(1, instanceCount_[blendIndex], 1);
}


void TriangleDrawer3D::AddTriangle(const Matrix4x4& worldMatrix, const TriangleData3D& data, const MaterialData3D& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendMode);

#if defined(DEBUG) || defined(DEVELOP)
	if (currentIndex_[blendIndex] >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("TriangleDrawer3D: Max instance count exceeded!\n");
		return;
	}
#endif // _DEBUG

	// テクスチャ名を取得
	std::string textureName = material.textureName;

	// 設定されていなければデフォルトのテクスチャを設定
	if (textureName == "") {
		textureName = "EngineAssets/Images/uvChecker.png";
	}

	TriangleData3DForGPU newTriangleData{
		.worldMatrix = worldMatrix,
		.worldInverseTranspose = MakeInverseTransposeMatrix(worldMatrix),
		.Offsets{
				Vector4(data.verticesOffsets[0].x, data.verticesOffsets[0].y, data.verticesOffsets[0].z, 1.0f),
				Vector4(data.verticesOffsets[1].x, data.verticesOffsets[1].y, data.verticesOffsets[1].z, 1.0f),
				Vector4(data.verticesOffsets[2].x, data.verticesOffsets[2].y, data.verticesOffsets[2].z, 1.0f),
		}
	};

	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = MAGISYSTEM::GetTextureIndex(textureName),
		.baseColor = material.baseColor,
		.uvMatrix = MakeUVMatrix(material.uvScale, material.uvRotate, material.uvTranslate),
	};

	instancingData_[blendIndex][currentIndex_[blendIndex]] = newTriangleData;
	materialData_[blendIndex][currentIndex_[blendIndex]] = newMaterialData;
	currentIndex_[blendIndex]++;
}

void TriangleDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void TriangleDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void TriangleDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void TriangleDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void TriangleDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}