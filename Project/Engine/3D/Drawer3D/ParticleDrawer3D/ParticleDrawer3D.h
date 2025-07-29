#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"


class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class ShadowPipelineManager;
class Camera3DManager;
class LightManager;
class ParticleUpdater3D;

/// <summary>
/// パーティクル3D描画クラス
/// </summary>
class ParticleEffectDrawer3D {
public:
	ParticleEffectDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		ShadowPipelineManager* shadowPipelineManager,
		Camera3DManager* camera3DManager,
		LightManager* lightManager,
		ParticleUpdater3D* particleUpdater
	);
	~ParticleEffectDrawer3D() = default;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BlendMode mode);

private:

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	ShadowPipelineManager* shadowPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
	LightManager* lightManager_ = nullptr;
	ParticleUpdater3D* particleUpdater_ = nullptr;
};