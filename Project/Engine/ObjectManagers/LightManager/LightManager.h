#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/LightStruct.h"
#include "Math/Utility/MathUtility.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;

/// <summary>
/// ライト管理クラス
/// </summary>
class LightManager {
public:
	LightManager(DXGI* dxgi, DirectXCommand* directXCommand);
	~LightManager();

	void Update();

	void TransferDirectionalLight(uint32_t paramIndex);
	void SetDirectionalLight(const DirectionalLight& directionalLight);

	void TransferDirectionalLightCamera(uint32_t paramIndex);

private:
	void CreateDirectionalLightResource();
	void MapDirectionalLightData();

	void CreateDirectionalLightCameraResource();
	void MapDirectionalLightCameraData();

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSrvUavManager(SRVUAVManager* srvuavManager);

private:
	//========================
	// DirectionalLightCamera
	//========================
	ComPtr<ID3D12Resource> directionalLightCameraResource_;
	DirectionalLightCameraForGPU* directionalLightCameraData_ = nullptr;

	// ニアクリップ距離
	const float nearClipRange_ = 1.0f;
	// ファークリップ距離
	const float farClipRange_ = 500.0f;

	// プロジェクション行列
	Matrix4x4 lightProj_{};

	//========================
	// DirectionalLight
	//========================
	DirectionalLight directionalLight_{};
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLightForGPU* directionalLightData_ = nullptr;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvuavManager_ = nullptr;
};