#pragma once

// C++
#include <string>
#include <vector>
#include <unordered_map>

// MyHedder
#include "Math/Types/Vector3.h"
#include "Math/Types/Quaternion.h"

struct SceneCameraData {
	std::string name;
	Vector3 eye;
	Vector3 target;
	Vector3 up = { 0,1,0 };
	float fovY;
	float nearClip;
	float farClip;
	std::vector<Vector3> eyeControlPoints;
	std::vector<Vector3> targetControlPoints;
};

struct SceneObjectData {
	std::string objectName;
	std::string modelName;
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
	bool isVisible;
};

struct SceneData {
	std::string name;
	std::vector<SceneObjectData> objects;
	std::vector<SceneCameraData> cameras;
};

/// <summary>
/// シーンデータコンテナ
/// </summary>
class SceneDataContainer {
public:
	SceneDataContainer();
	~SceneDataContainer();

	// シーンデータをロード
	void LoadBlenderLevelDataFromJson(const std::string& fileName);

	// シーンデータを取得
	const SceneData& GetData(const std::string& dataName);

private:
	// シーンデータコンテナ
	std::unordered_map<std::string, SceneData> sceneDatas_;

	const std::string kDirectoryPath_ = "Assets/SceneData/";
};