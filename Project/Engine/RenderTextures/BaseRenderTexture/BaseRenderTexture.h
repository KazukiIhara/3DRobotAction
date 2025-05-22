#pragma once

// C++
#include <cmath>
#include <cassert>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Enums/BlendModeEnum.h"
#include "Structs/ColorStruct.h"
#include "Math/Types/Vector4.h"

/// <summary>
/// レンダーテクスチャの基底クラス
/// </summary>
class BaseRenderTexture {
public:
	BaseRenderTexture();
	virtual ~BaseRenderTexture();

	// 初期化
	virtual void Initialize() = 0;

	// RtvIndexを取得
	uint32_t GetRtvIndex()const;

	// SrvIndexを取得
	uint32_t GetSrvIndex()const;

	// リソースを取得
	ID3D12Resource* GetResource();

	// バリアの状態を設定
	void TransitionToWrite();
	void TransitionToRead();

	// 自身をレンダーターゲットにする
	void SetAsRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE dsv = {});
	// クリア
	void ClearRenderTarget();
protected:
	void Create(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		Vector4 clearColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

private:
	// レンダーテクスチャのリソースを作成
	void CreateResource();
	// RTVを作成
	void CreateRTV();
	// SRVを作成
	void CreateSRV();

protected:
	// レンダーテクスチャ用のリソース
	ComPtr<ID3D12Resource> resource_ = nullptr;

	// 現在のリソースの状態
	D3D12_RESOURCE_STATES currentResourceState_ = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// フォーマット
	DXGI_FORMAT format_{};
	// リソースフラグ
	D3D12_RESOURCE_FLAGS resourceFlags_{};
	// クリアカラー
	Vector4 clearColor_{};

	// RTVリソースのインデックス
	uint32_t rtvIndex_ = 0;
	// SRVリソースのインデックス
	uint32_t srvIndex_ = 0;
};