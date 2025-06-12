#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyHedder
#include "3D/Cameras3D/DebugCamera3D/DebugCamera3D.h"

/// <summary>
/// 3D描画用カメラのマネージャ
/// </summary>
class Camera3DManager {
public:
	Camera3DManager();
	~Camera3DManager();

	void Initialize();
	void Update();
	void TransferCurrentCamera(uint32_t rootParameterIndex);
	void TransferCurrentCameraInverse(uint32_t rootParameterIndex);
	void TransferCurrentCameraFrustum(uint32_t rootParameterIndex);

	void DrawCurrentCameraFrustum();
	void ShakeCurrentCamera(float duration, float intensity);

	Camera3D* Add(const std::string& name, std::unique_ptr<Camera3D> newCamera3D);
	void Remove(const std::string& cameraName);
	Camera3D* Find(const std::string& cameraName);

	// 現在使用するカメラの名前をセット
	void SetCurrentCamera(const std::string& cameraName);

	// 使用中のカメラを取得
	Camera3D* GetCurrentCamera();

	// デバッグカメラフラグの参照
	bool& GetIsDebugCamera();

	void Clear();

private:
	// デバッグカメラ切り替え変数
	bool isDebugCamera_ = false;
	// カメラコンテナ
	std::map<std::string, std::unique_ptr<Camera3D>> cameras3D_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera3D> debugCamera_;
	// 現在使用中のカメラ
	Camera3D* currentCamera_ = nullptr;
};