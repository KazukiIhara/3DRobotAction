#include "SwapChain.h"

#include "Logger/Logger.h"

#include "WindowApp/WindowApp.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/RTVManager/RTVManager.h"

SwapChain::SwapChain(WindowApp* windowApp, DXGI* dxgi, Viewport* viewport, ScissorRect* scissorRect, DirectXCommand* command, RTVManager* rtvManager) {
	// WindowAppのインスタンスをセット
	SetWindowApp(windowApp);
	// DXGIのインスタンスをセット
	SetDXGI(dxgi);
	// Viewportのインスタンスをセット
	SetViewport(viewport);
	// ScissorRectのインスタンスをセット
	SetScissorRect(scissorRect);
	// Commandのセット
	SetCommand(command);
	// RTVManagerのインスタンスをセット
	SetRTVManager(rtvManager);

	// スワップチェーン作成
	CreateSwapChain();
	// リソースを作成
	CreateResources();
	// RTVを作成
	CreateRTV();

	// クリアカラーを設定
	clearColor_ = { 0.0f,0.0f,0.0f,1.0f };	Logger::Log("SwapChain Initialize\n");
}

SwapChain::~SwapChain() {
	Logger::Log("SwapChain Finalize\n");
}

void SwapChain::Present() {
	swapChain_->Present(1, 0);
}

ID3D12Resource* SwapChain::GetCurrentBackBufferResource() {
	backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();
	return swapChainResources_[backBufferIndex_].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetCurrentBackBufferRTVHandle() {
	backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();
	return rtvManager_->GetDescriptorHandleCPU(rtvIndex_[backBufferIndex_]);
}

void SwapChain::PreRender() {
	// スワップチェーンを書き込み可能の状態に
	TransitionToWrite();
	// スワップチェーンをレンダーターゲットに
	SetAsRenderTarget();
	// スワップチェーンをクリア
	ClearRenderTarget();
	// ビューポートの設定
	viewport_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();
}

void SwapChain::TransitionToWrite() {
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = GetCurrentBackBufferResource();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	directXCommand_->GetList()->ResourceBarrier(1, &barrier);
}

void SwapChain::TransitionToPresent() {
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = GetCurrentBackBufferResource();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	directXCommand_->GetList()->ResourceBarrier(1, &barrier);
}

void SwapChain::SetAsRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE dsv) {
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorHandle = rtvManager_->GetDescriptorHandleCPU(rtvIndex_[backBufferIndex_]);
	if (dsv.ptr == 0) {
		directXCommand_->GetList()->OMSetRenderTargets(1, &rtvDescriptorHandle, FALSE, nullptr);
	} else {
		directXCommand_->GetList()->OMSetRenderTargets(1, &rtvDescriptorHandle, FALSE, &dsv);
	}
}

void SwapChain::ClearRenderTarget() {
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorHandle = rtvManager_->GetDescriptorHandleCPU(rtvIndex_[backBufferIndex_]);
	const float clear[] = { clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w };
	directXCommand_->GetList()->ClearRenderTargetView(rtvDescriptorHandle, clear, 0, nullptr);
}

void SwapChain::CreateSwapChain() {
	// スワップチェーンの設定
	swapChainDesc_.Width = WindowApp::kClientWidth;				// 画面の幅、ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = WindowApp::kClientHeight;			// 画面の高さ、ウィンドウのクライアント領域を同じものしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// 色形式
	swapChainDesc_.SampleDesc.Count = 1;						// マルチサンプルしない
	swapChainDesc_.BufferCount = 2;								// ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタにうつしたら、中身を破棄
	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr_ = dxgi_->GetFactory()->CreateSwapChainForHwnd(directXCommand_->GetQueue(), windowApp_->GetHwnd(), &swapChainDesc_,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr_));
}

void SwapChain::CreateResources() {
	// スワップチェーンからリソースを引っ張ってくる
	hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	// うまく取得出来なければ起動できない
	assert(SUCCEEDED(hr_));
	hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr_));
}

void SwapChain::CreateRTV() {
	// 1つめ
	rtvIndex_[0] = rtvManager_->Allocate();
	rtvManager_->CreateRTVTexture2d(rtvIndex_[0], swapChainResources_[0].Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	// 2つめ
	rtvIndex_[1] = rtvManager_->Allocate();
	rtvManager_->CreateRTVTexture2d(rtvIndex_[1], swapChainResources_[1].Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
}

void SwapChain::SetWindowApp(WindowApp* windowApp) {
	assert(windowApp);
	windowApp_ = windowApp;
}

void SwapChain::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SwapChain::SetViewport(Viewport* viewport) {
	assert(viewport);
	viewport_ = viewport;
}

void SwapChain::SetScissorRect(ScissorRect* scissorRect) {
	assert(scissorRect);
	scissorRect_ = scissorRect;
}

void SwapChain::SetCommand(DirectXCommand* command) {
	assert(command);
	directXCommand_ = command;
}

void SwapChain::SetRTVManager(RTVManager* rtvManager) {
	assert(rtvManager);
	rtvManager_ = rtvManager;
}
