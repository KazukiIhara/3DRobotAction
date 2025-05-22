#pragma once

// C++
#include <unordered_map>
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/PunctualLightStruct.h"
#include "Enums/PunctualLightDataEnum.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;

class PunctualLightManager {
public:
	PunctualLightManager(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvuavManager);
	~PunctualLightManager();

	void Initialize(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvuavManager);
	void Update();
	void Clear();

	void TransferLightsData(uint32_t parmIndex);

	void AddNewLight(const std::string& lightName, const PunctualLightData& lightData);

	PunctualLightData& GetPunctualLight(const std::string& lightName);

	void DeleteGarbages();
private:
	void CreateLightsResource();
	void MapLightsData();

	void CreateLightCountResource();
	void MapLightCountData();

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSrvUavManager(SRVUAVManager* srvuavManager);
private:
	// ライトの最大数
	const uint32_t kMaxLightNum_ = 100;

	// ライト
	std::unordered_map<std::string, PunctualLightData> light_;

	// ライトのリソース(instancing用)
	ComPtr<ID3D12Resource> lightsResource_ = nullptr;
	// GPUに送るライトのデータ
	PunctualLightDataForGPU* lightsData_ = nullptr;
	// SrvIndex
	uint32_t srvIndex_ = 0;

	// ライトの数を送るリソース
	ComPtr<ID3D12Resource> lightCountResource_ = nullptr;
	// GPUに送るライトの数のデータ
	PunctualLightCount* lightCountData_ = nullptr;
	// 現在のライト数
	uint32_t currentLightNum_ = 0;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvuavManager_ = nullptr;
};