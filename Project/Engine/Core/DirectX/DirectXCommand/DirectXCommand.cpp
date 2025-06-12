#include "DirectXCommand.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"

#include "Logger/Logger.h"

DirectXCommand::DirectXCommand(DXGI* dxgi, bool isSupportDirectX12Ultimate) {
	Initialize(dxgi, isSupportDirectX12Ultimate);
	Logger::Log("DirectXCommand Initialize\n");
}

DirectXCommand::~DirectXCommand() {
	Logger::Log("DirectXCommand Finalize\n");
}

void DirectXCommand::Initialize(DXGI* dxgi, bool isSupportDirectX12Ultimate) {
	// DXGIのセット
	SetDXGI(dxgi);
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr_ = dxgi_->GetDevice()->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr_));

	// コマンドアロケータを生成する
	hr_ = dxgi_->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr_));

	// コマンドリストを生成する
	hr_ = dxgi_->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr_));

	// もしサポートされているデバイスなら
	if (isSupportDirectX12Ultimate) {
		// MeshShader用：ID3D12GraphicsCommandList6 にキャスト
		hr_ = commandList_->QueryInterface(IID_PPV_ARGS(&commandList6_));
		assert(SUCCEEDED(hr_));
	}
}

void DirectXCommand::KickCommand() {
	// 積んだコマンドをクローズ
	commandList_->Close();
	// GPUにコマンドリストの実行を行わせる
	ComPtr<ID3D12CommandList> commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());
}

void DirectXCommand::ResetCommand() {
	// アロケータのリセット
	hr_ = commandAllocator_->Reset();
	assert(SUCCEEDED(hr_));
	// コマンドリストのリセット
	hr_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr_));
}

ID3D12CommandQueue* DirectXCommand::GetQueue() {
	return commandQueue_.Get();
}

ID3D12CommandAllocator* DirectXCommand::GetAllocator() {
	return commandAllocator_.Get();
}

ID3D12GraphicsCommandList* DirectXCommand::GetList() {
	return commandList_.Get();
}

ID3D12GraphicsCommandList6* DirectXCommand::GetList6() {
	return commandList6_.Get();
}

void DirectXCommand::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}
