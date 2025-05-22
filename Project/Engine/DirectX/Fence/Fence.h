#pragma once

// C++
#include <cstdint>
#include <cassert>

// DirectX
#include <d3d12.h>

// ComPtr
#include "DirectX/ComPtr/ComPtr.h"

class DXGI;
class DirectXCommand;

class Fence {
public:
	Fence(DXGI* dxgi, DirectXCommand* directXCommand);
	~Fence();

	// 初期化
	void Initialize(DXGI* dxgi, DirectXCommand* directXCommand);
	// GPUを待機
	void WaitGPU();
private:
	// フェンス作成
	void CreateFence();
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* directXCommand);
private:
	// フェンス
	ComPtr<ID3D12Fence> fence_ = nullptr;
	// フェンスバリュー
	UINT64 fenceValue_ = 0;
	// エラー判別君
	HRESULT hr_ = S_FALSE;
private:
	// DXGIのインスタンスを受け取る箱
	DXGI* dxgi_ = nullptr;
	// Commandのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
};