#pragma once
#include <cstdint>

#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"

class DXGI;
class DirectXCommand;
class DSVManager;
class SRVUAVManager;

class DepthStencil {
public:
	DepthStencil(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager, SRVUAVManager* srvUavManager);
	~DepthStencil();

	// 初期化
	void Initialize(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager, SRVUAVManager* srvUavManager);
	// デプスをクリア
	void ClearDepthView();

	void TransitionToWrite();
	void TransitionToRead();

	// デプスステンシルリソースのCPUハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilResorceCPUHandle();
	
	// Srvインデックスを取得
	uint32_t GetSrvIndex()const;

private:
	// デプスステンシルリソースの作成
	void CreateResource();
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// DSVマネージャのインスタンスをセット
	void SetDSVManager(DSVManager* dsvManager);
	// SRVUAVマネージャのインスタンスをセット
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
private:
	// リソース
	ComPtr<ID3D12Resource> resource_ = nullptr;
	D3D12_RESOURCE_STATES currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	// dsvインデックス
	uint32_t dsvIndex_ = 0;
	// srvインデックス
	uint32_t srvIndex_ = 0;

private:
	// DXGIのインスタンスを受け取る箱
	DXGI* dxgi_ = nullptr;
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// DSVマネージャのインスタンスを受け取る箱
	DSVManager* dsvManager_ = nullptr;
	// SRVUAVマネージャのインスタンスを受け取る箱
	SRVUAVManager* srvUavManager_ = nullptr;
};