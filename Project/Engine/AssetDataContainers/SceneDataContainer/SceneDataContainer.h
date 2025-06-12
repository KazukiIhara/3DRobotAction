#pragma once

// C++
#include <string>
#include <vector>
#include <unordered_map>

// MyHedder
#include "Math/Types/Vector3.h"
#include "Math/Types/Quaternion.h"

struct SceneObjectData {
	std::string objectName;
	std::string modelName;
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct SceneData {
	std::string name;
	std::vector<SceneObjectData> objects;
};

/// <summary>
/// シーンデータコンテナ
/// </summary>
class SceneDataContainer {
public:
	SceneDataContainer();
	~SceneDataContainer();

	// シーンデータをロード
	void LoadFromJson(const std::string& fileName);

	// シーンデータを取得
	const SceneData& GetData(const std::string& dataName);

private:
	// シーンデータコンテナ
	std::unordered_map<std::string, SceneData> sceneDatas_;

	const std::string kDirectoryPath_ = "Assets/SceneData/";
};