#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

// ComPtr
#include "DirectX/ComPtr/ComPtr.h"

// 前方宣言
class DXGI;

class DirectXCommand {
public:
	DirectXCommand(DXGI* dxgi, bool isSupportDirectX12Ultimate);
	~DirectXCommand();

	// 初期化
	void Initialize(DXGI* dxgi, bool isSupportDirectX12Ultimate);
	// コマンドの実行
	void KickCommand();
	// コマンドのリセット
	void ResetCommand();
	// キューの取得
	ID3D12CommandQueue* GetQueue();
	// アロケータの取得
	ID3D12CommandAllocator* GetAllocator();
	// リストの取得
	ID3D12GraphicsCommandList* GetList();
	// メッシュシェーダー用リストの取得
	ID3D12GraphicsCommandList6* GetList6();
private:
	// DXGIのセット
	void SetDXGI(DXGI* dxgi);
private:
	// SUCCEEDEDでエラー判別君
	HRESULT hr_ = S_FALSE;
	// DXGI
	DXGI* dxgi_ = nullptr;
	// コマンドキュー
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	// コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	// コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	// MeshShader用コマンドリスト
	ComPtr<ID3D12GraphicsCommandList6> commandList6_ = nullptr;
};