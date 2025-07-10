#pragma once

// C++
#include <string>

// 前方宣言
class SceneDataContainer;
class GameObject3DManager;
class Camera3DManager;
class Renderer3DManager;
class TransformManager;

/// <summary>
/// シーンデータをインポートする
/// </summary>
class SceneDataImporter {
public:
	SceneDataImporter(SceneDataContainer* sceneDataContainer, GameObject3DManager* gameObject3DManager, Camera3DManager* camera3DManager, Renderer3DManager* renderer3DManager, TransformManager* transformManager);
	~SceneDataImporter();

	// シーンデータインポート
	void Import(const std::string& scaneDataName, bool isSceneClear);

private:
	SceneDataContainer* sceneDataContainer_ = nullptr;
	GameObject3DManager* gameObject3DManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
	Renderer3DManager* renderer3DManager_ = nullptr;
	TransformManager* transformManager_ = nullptr;
};