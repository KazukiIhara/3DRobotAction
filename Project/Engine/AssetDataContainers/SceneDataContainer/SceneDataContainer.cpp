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

void SceneDataContainer::LoadBlenderLevelDataFromJson(const std::string& fileName) {
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

	std::unordered_map<std::string, size_t> camIndex;

	using CPBuf = std::vector<std::pair<int, Vector3>>;
	std::unordered_map<std::string, CPBuf> pendingEye;
	std::unordered_map<std::string, CPBuf> pendingTgt;

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

			newObject.isVisible = object["visible"];

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

		} else if (type.compare("CAMERA") == 0) {

			SceneCameraData newCamera;

			nlohmann::json& cameraData = object["camera_data"];

			newCamera.name = object["name"];

			newCamera.eye.x = static_cast<float>(cameraData["eye"][0]);
			newCamera.eye.y = static_cast<float>(cameraData["eye"][1]);
			newCamera.eye.z = static_cast<float>(cameraData["eye"][2]);

			newCamera.target.x = static_cast<float>(cameraData["target"][0]);
			newCamera.target.y = static_cast<float>(cameraData["target"][1]);
			newCamera.target.z = static_cast<float>(cameraData["target"][2]);

			newCamera.fovY = static_cast<float>(cameraData["fovY"]);

			newCamera.nearClip = static_cast<float>(cameraData["nearClip"]);
			newCamera.farClip = static_cast<float>(cameraData["farClip"]);

			newSceneData.cameras.push_back(newCamera);

		} else if (type == "EMPTY") {

			if (!object.contains("identifier") ||
				!object.contains("order") ||
				!object.contains("camera_name") ||
				!object.contains("transform"))
				continue;   // 情報不足 → 無視

			const std::string id = object["identifier"];
			const int ord = object["order"];
			const std::string cam = object["camera_name"];

			Vector3 pos{
				object["transform"]["translate"][0],
				object["transform"]["translate"][1],
				object["transform"]["translate"][2]
			};

			if (id == "CAMERA_EYE")
				pendingEye[cam].emplace_back(ord, pos);
			else if (id == "CAMERA_TARGET")
				pendingTgt[cam].emplace_back(ord, pos);
		}

	}


	// コントロールポイントを一時保存リストからカメラに追加
	auto sortByOrder = [](auto& a, auto& b) { return a.first < b.first; };

	for (auto& cam : newSceneData.cameras) {
		auto& eyeBuf = pendingEye[cam.name];
		auto& tgtBuf = pendingTgt[cam.name];

		std::sort(eyeBuf.begin(), eyeBuf.end(), sortByOrder);
		std::sort(tgtBuf.begin(), tgtBuf.end(), sortByOrder);

		cam.eyeControlPoints.reserve(eyeBuf.size());
		cam.targetControlPoints.reserve(tgtBuf.size());

		for (auto& [o, p] : eyeBuf) cam.eyeControlPoints.push_back(p);
		for (auto& [o, p] : tgtBuf) cam.targetControlPoints.push_back(p);
	}

	// コンテナに登録
	sceneDatas_.insert_or_assign(newSceneData.name, newSceneData);
}

const SceneData& SceneDataContainer::GetData(const std::string& dataName) {
	auto it = sceneDatas_.find(dataName);
	MAGIAssert::Assert(it != sceneDatas_.end(), "SceneData not found! DataName: " + dataName);
	return it->second;
}
