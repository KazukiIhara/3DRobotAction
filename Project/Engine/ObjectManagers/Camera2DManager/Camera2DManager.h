#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyHedder
#include "2D/Camera2D/Camera2D.h"

/// <summary>
/// 2D描画用のカメラマネージャ
/// </summary>
class Camera2DManager {
public:
	Camera2DManager();
	~Camera2DManager();

	void Initialize();
	void Update();
	void TransferCurrentCamera(uint32_t rootParameterIndex);

	void Add(std::unique_ptr<Camera2D> newCamera2D);
	Camera2D* Find(const std::string& cameraName);

	// 使用するカメラを指定
	void SetCurrentCamera(const std::string& cameraName);

	void Clear();
private:
	// カメラコンテナ
	std::map<std::string, std::unique_ptr<Camera2D>> cameras2D_;

	// 現在使用中のカメラ
	Camera2D* currentCamera_ = nullptr;
};