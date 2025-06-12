#include "SceneDataContainer.h"

// C++
#include <fstream>
#include <cassert>

// Json
#include <nlohmann/json.hpp>

// MyHedder
#include "MAGIAssert/MAGIAssert.h"

SceneDataContainer::SceneDataContainer() {
	sceneDatas_.clear();
	Logger::Log("SceneDataContainer Initialize\n");
}

SceneDataContainer::~SceneDataContainer() {
	sceneDatas_.clear();
	Logger::Log("SceneDataContainer Finalize\n");
}

void SceneDataContainer::LoadFromJson(const std::string& fileName) {
	// 今回ぶち込むシーンデータ
	SceneData newSceneData;
	newSceneData.name = fileName;

	// フルパス作成
	const std::string fullPath = kDirectoryPath_ + fileName + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);
	// ファイルオープン失敗をチェック
	MAGIAssert::Assert(!file.fail(), "Json SceneData not found! FileName: " + fileName);

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として登録
	std::string name =
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHの読み込み
		if (type.compare("MESH") == 0) {

			SceneObjectData newObject;

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			newObject.objectName = object["name"];
			newObject.modelName = object["model_name"];

			newObject.scale.x = static_cast<float>(transform["scale"][0]);
			newObject.scale.y = static_cast<float>(transform["scale"][1]);
			newObject.scale.z = static_cast<float>(transform["scale"][2]);

			newObject.rotate.x = static_cast<float>(transform["rotate"][0]);
			newObject.rotate.y = static_cast<float>(transform["rotate"][1]);
			newObject.rotate.z = static_cast<float>(transform["rotate"][2]);
			newObject.rotate.w = static_cast<float>(transform["rotate"][3]);

			newObject.translate.x = static_cast<float>(transform["translate"][0]);
			newObject.translate.y = static_cast<float>(transform["translate"][1]);
			newObject.translate.z = static_cast<float>(transform["translate"][2]);

			newSceneData.objects.push_back(newObject);
		}

	}

	// コンテナに登録
	sceneDatas_.insert_or_assign(newSceneData.name, newSceneData);
}

const SceneData& SceneDataContainer::GetData(const std::string& dataName) {
	auto it = sceneDatas_.find(dataName);
	MAGIAssert::Assert(it != sceneDatas_.end(), "SceneData not found! DataName: " + dataName);
	return it->second;
}
