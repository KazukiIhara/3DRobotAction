#include "LightManager.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;
using namespace MAGIUtility;

LightManager::LightManager(DXGI* dxgi, DirectXCommand* directXCommand) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);

	CreateDirectionalLightResource();
	MapDirectionalLightData();

	CreateDirectionalLightCameraResource();
	MapDirectionalLightCameraData();

	CreateDirectionalLightFrustumResource();
	MapDirectionalLightFrustumData();

	lightProj_ = MakeOrthographicMatrix(120.0f, 120.0f, nearClipRange_, farClipRange_);

	Logger::Log("LightManager Initialize\n");
}

LightManager::~LightManager() {
	Logger::Log("LightManager Finalize\n");
}

void LightManager::Update() {
	// ライト情報の更新
	directionalLightData_->direction = directionalLight_.direction;
	directionalLightData_->intensity = directionalLight_.intensity;
	directionalLightData_->color = directionalLight_.color;

	// ----------------------------------------------
	// シャドウマップ用ライトカメラ行列の構築
	// ----------------------------------------------

	// ライト方向の正規化
	const Vector3 lightDir = Normalize(directionalLight_.direction);

	// シーン中心から離れた位置にライト（eye）を置く
	const float lightDistance = 100.0f;

	Vector3 up = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 position = target_ - lightDir * lightDistance;

	// ビュー行列（ライト空間ビュー）
	lightView_ = MakeLookAtMatrix(position, target_, up);

	// VP 行列を GPU 定数バッファへ書き込み
	directionalLightCameraData_->viewProjection = lightView_ * lightProj_;

	const Matrix4x4 vp = lightView_ * lightProj_;
	auto row = [&](int r, int c) { return vp.m[r][c]; };

	frustumPlanes_[0] = NormalizePlane({ row(0,3) + row(0,0),
										 row(1,3) + row(1,0),
										 row(2,3) + row(2,0),
										 row(3,3) + row(3,0) }); // Left
	frustumPlanes_[1] = NormalizePlane({ row(0,3) - row(0,0),
										 row(1,3) - row(1,0),
										 row(2,3) - row(2,0),
										 row(3,3) - row(3,0) }); // Right
	frustumPlanes_[2] = NormalizePlane({ row(0,3) + row(0,1),
										 row(1,3) + row(1,1),
										 row(2,3) + row(2,1),
										 row(3,3) + row(3,1) }); // Bottom
	frustumPlanes_[3] = NormalizePlane({ row(0,3) - row(0,1),
										 row(1,3) - row(1,1),
										 row(2,3) - row(2,1),
										 row(3,3) - row(3,1) }); // Top
	frustumPlanes_[4] = NormalizePlane({ row(0,2),
										 row(1,2),
										 row(2,2),
										 row(3,2) });          // Near
	frustumPlanes_[5] = NormalizePlane({ row(0,3) - row(0,2),
										 row(1,3) - row(1,2),
										 row(2,3) - row(2,2),
										 row(3,3) - row(3,2) }); // Far

	for (int i = 0; i < 6; ++i) {
		directionalLightFrustumData_->planes[i] = frustumPlanes_[i];
	}

}

void LightManager::SetDirectionalLight(const DirectionalLight& directionalLight) {
	directionalLight_.direction = Normalize(directionalLight.direction);
	directionalLight_.intensity = directionalLight.intensity;
	directionalLight_.color = directionalLight.color;
}

void LightManager::SetDirectionalLightCameraTarget(const Vector3& target) {
	target_ = target;
}

void LightManager::TransferDirectionalLight(uint32_t paramIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライト情報を送る
	commandList->SetGraphicsRootConstantBufferView(paramIndex, directionalLightResource_->GetGPUVirtualAddress());
}

void LightManager::TransferDirectionalLightCamera(uint32_t paramIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライト情報を送る
	commandList->SetGraphicsRootConstantBufferView(paramIndex, directionalLightCameraResource_->GetGPUVirtualAddress());
}

void LightManager::TransferDirectionalLightFrustum(uint32_t paramIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライト情報を送る
	commandList->SetGraphicsRootConstantBufferView(paramIndex, directionalLightFrustumResource_->GetGPUVirtualAddress());
}

void LightManager::CreateDirectionalLightResource() {
	directionalLightResource_ = dxgi_->CreateBufferResource(sizeof(DirectionalLightForGPU));
}

void LightManager::MapDirectionalLightData() {
	directionalLightData_ = nullptr;
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
	directionalLightData_->color = { 1.0f,1.0f,1.0f };
}

void LightManager::CreateDirectionalLightCameraResource() {
	directionalLightCameraResource_ = dxgi_->CreateBufferResource(sizeof(DirectionalLightCameraForGPU));
}

void LightManager::MapDirectionalLightCameraData() {
	directionalLightCameraData_ = nullptr;
	directionalLightCameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightCameraData_));
	directionalLightCameraData_->viewProjection = MakeIdentityMatrix4x4();
}

void LightManager::CreateDirectionalLightFrustumResource() {
	directionalLightFrustumResource_ = dxgi_->CreateBufferResource(sizeof(DirectionalLightFrustumForGPU));
}

void LightManager::MapDirectionalLightFrustumData() {
	directionalLightFrustumData_ = nullptr;
	directionalLightFrustumResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightFrustumData_));
	for (uint32_t i = 0; i < 6; i++) {
		directionalLightFrustumData_->planes[i] = { 0.0f,0.0f,0.0f };
	}
}

void LightManager::DrawDirectionalLightFrustum() {
	// NDC空間の8頂点（Z: [0=Near, 1=Far]、左手系）
	const Vector3 ndcCorners[8] =
	{
		{ -1,  1, 0 }, // 0: Near-Top-Left
		{  1,  1, 0 }, // 1: Near-Top-Right
		{ -1, -1, 0 }, // 2: Near-Bottom-Left
		{  1, -1, 0 }, // 3: Near-Bottom-Right
		{ -1,  1, 1 }, // 4: Far-Top-Left
		{  1,  1, 1 }, // 5: Far-Top-Right
		{ -1, -1, 1 }, // 6: Far-Bottom-Left
		{  1, -1, 1 }, // 7: Far-Bottom-Right
	};

	// ビュー・プロジェクション逆行列を個別に取得
	const Matrix4x4 invView = Inverse(lightView_);
	const Matrix4x4 invProj = Inverse(lightProj_);

	// ワールド空間のFrustum頂点
	Vector3 worldCorners[8];
	for (int i = 0; i < 8; ++i) {
		Vector3 ndc = ndcCorners[i];

		// Clip空間座標
		Vector4 clip = Vector4(ndc.x, ndc.y, ndc.z, 1.0f);

		// View空間へ
		Vector4 view = Transform(clip, invProj);
		if (abs(view.w) > 1e-5f) {
			view /= view.w;
		}

		// ワールド空間へ
		Vector4 world = Transform(view, invView);
		worldCorners[i] = Vector3(world.x, world.y, world.z) / world.w;
	}

	// 色指定
	const Vector4 color = Color::Red;

	// Near平面
	MAGISYSTEM::DrawLine3D(worldCorners[0], worldCorners[1], color);
	MAGISYSTEM::DrawLine3D(worldCorners[1], worldCorners[3], color);
	MAGISYSTEM::DrawLine3D(worldCorners[3], worldCorners[2], color);
	MAGISYSTEM::DrawLine3D(worldCorners[2], worldCorners[0], color);

	// Far平面
	MAGISYSTEM::DrawLine3D(worldCorners[4], worldCorners[5], color);
	MAGISYSTEM::DrawLine3D(worldCorners[5], worldCorners[7], color);
	MAGISYSTEM::DrawLine3D(worldCorners[7], worldCorners[6], color);
	MAGISYSTEM::DrawLine3D(worldCorners[6], worldCorners[4], color);

	// Near↔Farの辺
	MAGISYSTEM::DrawLine3D(worldCorners[0], worldCorners[4], color);
	MAGISYSTEM::DrawLine3D(worldCorners[1], worldCorners[5], color);
	MAGISYSTEM::DrawLine3D(worldCorners[2], worldCorners[6], color);
	MAGISYSTEM::DrawLine3D(worldCorners[3], worldCorners[7], color);

}

void LightManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void LightManager::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void LightManager::SetSrvUavManager(SRVUAVManager* srvuavManager) {
	assert(srvuavManager);
	srvuavManager_ = srvuavManager;
}
