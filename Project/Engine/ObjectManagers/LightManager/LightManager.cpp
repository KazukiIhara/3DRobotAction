#include "LightManager.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
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

	lightProj_ = MakeOrthographicMatrix(100.0f, 100.0f, nearClipRange_, farClipRange_);

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

	Vector3 position = target_ - lightDir * lightDistance;

	// ビュー行列（ライト空間ビュー）
	lightView_ = MakeLookAtMatrix(position, target_);

	// VP行列とワールド座標をGPU定数バッファへ書き込み
	const Matrix4x4 vp = lightView_ * lightProj_;
	directionalLightCameraData_->viewProjection = vp;
	directionalLightCameraData_->position = position;

	// 視推台を作る
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
										 row(3,2) });            // Near
	frustumPlanes_[5] = NormalizePlane({ row(0,3) - row(0,2),
										 row(1,3) - row(1,2),
										 row(2,3) - row(2,2),
										 row(3,3) - row(3,2) }); // Far
	// 視推台を定数バッファに書き込み
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
	directionalLightCameraData_->position = ExtractionWorldPos(MakeIdentityMatrix4x4());
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
