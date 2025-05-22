#include "LineDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

using namespace MAGIUtility;

LineDrawer3D::LineDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);
	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// srvのインデックスを割り当て
	srvIndex_ = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(LineData3D));

	lines_.reserve(PrimitiveCommonConst::NumMaxInstance);
	Logger::Log("LineDrawer3D Initialize\n");
}

LineDrawer3D::~LineDrawer3D() {
	Logger::Log("LineDrawer3D Finalize\n");
}

void LineDrawer3D::Update() {
	// 最大数を超えていたら止める
	if (lines_.size() > PrimitiveCommonConst::NumMaxInstance) {
		assert(false && "Line size is over !");
	}

	// 描画すべきインスタンス数
	instanceCount_ = static_cast<uint32_t>(lines_.size());

	if (instancingData_ != nullptr && !lines_.empty()) {
		// コピー
		std::memcpy(instancingData_, lines_.data(), instanceCount_ * sizeof(LineData3D));
	}
	// ラインのコンテナをクリア
	ClearLines();
}

void LineDrawer3D::Draw() {
	if (instanceCount_ == 0) return;

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Line3D));
	// PSOを設定
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Line3D, blendMode_));

	// 形状設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// Cameraを転送
	camera3DManager_->TransferCurrentCamera(0);
	// StructuredBufferのSRVを設定する
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(srvIndex_));
	// 描画
	commandList->DrawInstanced(2, instanceCount_, 0, 0);
}

void LineDrawer3D::AddLine(const Vector3& start, const Vector3& end, const Vector4& color) {
	// 追加するLine
	LineData3D newLineData{
		.start = start,
		.end = end,
		.color = color,
	};
	// コンテナに挿入
	lines_.push_back(newLineData);
}

void LineDrawer3D::ClearLines() {
	lines_.clear();
}

void LineDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void LineDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void LineDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void LineDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void LineDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void LineDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(LineData3D) * PrimitiveCommonConst::NumMaxInstance);
}

void LineDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < PrimitiveCommonConst::NumMaxInstance; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}
