#include "DepthStencil.h"

#include <cassert>

#include "Logger/Logger.h"

#include "WindowApp/WindowApp.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/DSVManager/DSVManager.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

DepthStencil::DepthStencil(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager, SRVUAVManager* srvUavManager) {
	Initialize(dxgi, command, dsvManager, srvUavManager);
	Logger::Log("DepthStencil Initialize\n");
}

DepthStencil::~DepthStencil() {
	Logger::Log("DepthStencil Finalize\n");
}

void DepthStencil::Initialize(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager, SRVUAVManager* srvUavManager) {
	// DXGIのセット
	SetDXGI(dxgi);
	// コマンドをセット
	SetCommand(command);
	// DSVマネージャをセット
	SetDSVManager(dsvManager);
	// SRVマネージャをセット
	SetSRVUAVManager(srvUavManager);
	// DepthStencilViewを作成
	CreateResource();
}

void DepthStencil::ClearDepthView() {
	// 指定した深度で画面全体をクリア
	directXCommand_->GetList()->ClearDepthStencilView(
		dsvManager_->GetDescriptorHandleCPU(dsvIndex_),
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);
}

void DepthStencil::TransitionToWrite() {
	if (currentResourceState_ != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = resource_.Get();
		barrier.Transition.StateBefore = currentResourceState_;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		directXCommand_->GetList()->ResourceBarrier(1, &barrier);

		currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	}
}

void DepthStencil::TransitionToRead() {
	// DEPTH_READ | PIXEL_SHADER_RESOURCE の複合ビットへ
	const auto desired =
		D3D12_RESOURCE_STATE_DEPTH_READ |
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	if (currentResourceState_ != desired) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = resource_.Get();
		barrier.Transition.StateBefore = currentResourceState_;
		barrier.Transition.StateAfter = desired;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		directXCommand_->GetList()->ResourceBarrier(1, &barrier);
		currentResourceState_ = desired;
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE DepthStencil::GetDepthStencilResorceCPUHandle() {
	return dsvManager_->GetDescriptorHandleCPU(dsvIndex_);
}

uint32_t DepthStencil::GetSrvIndex() const {
	return srvIndex_;
}

void DepthStencil::CreateResource() {
	// DepthStencilTextureをウィンドウのサイズで作成
	resource_ = dxgi_->CreateDepthStencilTextureResource(
		WindowApp::kClientWidth,
		WindowApp::kClientHeight,
		DXGI_FORMAT_R24G8_TYPELESS,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);
	// インデックス割り当て
	dsvIndex_ = dsvManager_->Allocate();
	srvIndex_ = srvUavManager_->Allocate();

	// dsv作成
	dsvManager_->CreateDSVTexture2d(
		dsvIndex_,
		resource_.Get(),
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);

	// srv作成
	srvUavManager_->CreateSrvTexture2d(
		srvIndex_,
		resource_.Get(),
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS, 1
	);

	currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
}

void DepthStencil::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void DepthStencil::SetCommand(DirectXCommand* command) {
	assert(command);
	directXCommand_ = command;
}

void DepthStencil::SetDSVManager(DSVManager* dsvManager) {
	assert(dsvManager);
	dsvManager_ = dsvManager;
}

void DepthStencil::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}
