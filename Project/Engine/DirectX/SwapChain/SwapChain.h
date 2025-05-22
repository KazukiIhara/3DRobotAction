#pragma once

// C++
#include <cmath>
#include <cassert>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// 自作ファイル
#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Types/Vector4.h"

// 前方宣言
class WindowApp;
class DXGI;
class DirectXCommand;
class RTVManager;
class Viewport;
class ScissorRect;

class SwapChain {
public:
	SwapChain(WindowApp* windowApp, DXGI* dxgi, Viewport* viewport, ScissorRect* scissorRect, DirectXCommand* command, RTVManager* rtvManager);
	~SwapChain();

	// GPUとOSに画面の交換を行うよう通知する
	void Present();
	// 現在のバックバッファのリソースを取得
	ID3D12Resource* GetCurrentBackBufferResource();
	// 描画先のRTVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTVHandle();

	// スワップチェーン描画前処理
	void PreRender();

	// バリアの状態を設定
	void TransitionToWrite();
	void TransitionToPresent();

	// 自身をレンダーターゲットにする
	void SetAsRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE dsv = {});
	// クリア
	void ClearRenderTarget();
private:
	// スワップチェーン作成
	void CreateSwapChain();
	// スワップチェーンのRTVリソースを作成
	void CreateResources();
	// RTVを作成
	void CreateRTV();
private:
	// WindowAppをセット
	void SetWindowApp(WindowApp* windowApp);
	// DXGIをセット
	void SetDXGI(DXGI* dxgi);
	// Viewport
	void SetViewport(Viewport* viewport);
	// ScissorRect
	void SetScissorRect(ScissorRect* scissorRect);
	// Commandをセット
	void SetCommand(DirectXCommand* command);
	// RTVManagerのセット
	void SetRTVManager(RTVManager* rtvManager);
private:
	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	// スワップチェーンリソース
	ComPtr<ID3D12Resource> swapChainResources_[2] = { nullptr };
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// RTVリソースのインデックス
	uint32_t rtvIndex_[2]{};
	// バックバッファインデックス
	UINT backBufferIndex_ = 0;
	// クリアカラー
	Vector4 clearColor_{};
	// エラー判別君
	HRESULT hr_ = S_FALSE;
private:
	// WindowAppのインスタンスを受け取る箱
	WindowApp* windowApp_ = nullptr;
	// dxgiのインスタンスを受け取る箱
	DXGI* dxgi_ = nullptr;
	// viewportのインスタンスを受け取る箱
	Viewport* viewport_ = nullptr;
	// ScissorRectのインスタンスを受け取る箱
	ScissorRect* scissorRect_ = nullptr;
	// Commandのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// RTVmanagerのインスタンスを受け取る箱
	RTVManager* rtvManager_ = nullptr;
};