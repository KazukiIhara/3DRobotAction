#include "MechAnimationContainer.h"

// C++
#include <filesystem>
#include <fstream>
#include <algorithm>

// JSON（nlohmann）
#include <nlohmann/json.hpp>

namespace {
	namespace fs = std::filesystem;
	using json = nlohmann::json;

	// ファイル名に使えない文字を '_' にする
	std::string SanitizeFileName(std::string s) {
		for (char& c : s) {
			const bool bad =
				(c == '\\') || (c == '/') || (c == ':') || (c == '*') || (c == '?') ||
				(c == '"') || (c == '<') || (c == '>') || (c == '|');
			if (bad) {
				c = '_';
			}
		}
		return s;
	}

	// Pose -> json
	json PoseToJson(const MechAnimation::Pose& pose) {
		json j;
		j["rotations"] = json::array();

		// rotationsを書き出し
		for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
			const Quaternion& q = pose.rotations[i];
			json jq;
			jq["x"] = q.x;
			jq["y"] = q.y;
			jq["z"] = q.z;
			jq["w"] = q.w;
			j["rotations"].push_back(jq);
		}

		return j;
	}

	// json -> Pose
	bool JsonToPose(const json& j, MechAnimation::Pose& outPose) {
		if (!j.contains("rotations")) {
			return false;
		}
		if (!j["rotations"].is_array()) {
			return false;
		}

		const auto& arr = j["rotations"];
		const size_t n = std::min<size_t>(arr.size(), MechAnimation::kJointCount);

		// 不足分はidentity
		const Quaternion identity{ 0.0f,0.0f,0.0f,1.0f };
		outPose.rotations.fill(identity);

		for (size_t i = 0; i < n; ++i) {
			const auto& jq = arr[i];
			if (!jq.is_object()) {
				continue;
			}
			Quaternion q = identity;

			// 値取得
			if (jq.contains("x")) q.x = jq["x"].get<float>();
			if (jq.contains("y")) q.y = jq["y"].get<float>();
			if (jq.contains("z")) q.z = jq["z"].get<float>();
			if (jq.contains("w")) q.w = jq["w"].get<float>();

			outPose.rotations[i] = q;
		}

		return true;
	}

	// Clip -> json
	json ClipToJson(const std::string& name, const MechAnimation::Clip& clip) {
		json j;
		j["version"] = 1;
		j["name"] = name;
		j["jointCount"] = static_cast<int>(MechAnimation::kJointCount);

		j["frames"] = json::array();
		// framesを書き出し
		for (const auto& pose : clip.frames) {
			j["frames"].push_back(PoseToJson(pose));
		}

		return j;
	}

	// json -> Clip
	bool JsonToClip(const json& j, MechAnimation::Clip& outClip) {
		if (!j.contains("frames")) {
			return false;
		}
		if (!j["frames"].is_array()) {
			return false;
		}

		outClip.frames.clear();

		const auto& frames = j["frames"];
		outClip.frames.reserve(frames.size());

		// framesを復元
		for (const auto& jp : frames) {
			MechAnimation::Pose pose{};
			if (!JsonToPose(jp, pose)) {
				return false;
			}
			outClip.frames.push_back(pose);
		}

		return true;
	}
}

MechAnimationContainer::MechAnimationContainer() {
	LoadAllClips();
}

MechAnimationContainer::~MechAnimationContainer() {
	SaveAllClips();
}

bool MechAnimationContainer::AddClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite) {
	if (name.empty()) {
		return false;
	} // 名前チェック

	auto it = clips_.find(name);
	if (it != clips_.end()) {
		if (!overwrite) {
			return false;
		} // 上書き禁止
		it->second = clip; // 上書き
		return true;
	}

	clips_.emplace(name, clip); // 追加
	return true;
}

bool MechAnimationContainer::RemoveClip(const std::string& name) {
	auto it = clips_.find(name);
	if (it == clips_.end()) {
		return false;
	} // 存在チェック

	clips_.erase(it); // 削除
	return true;
}

bool MechAnimationContainer::HasClip(const std::string& name) const {
	return clips_.find(name) != clips_.end(); // 存在チェック
}

const MechAnimation::Clip* MechAnimationContainer::GetClip(const std::string& name) const {
	auto it = clips_.find(name);
	if (it == clips_.end()) {
		return nullptr;
	} // 取得失敗
	return &it->second; // 参照返し
}

MechAnimation::Clip* MechAnimationContainer::GetClipMutable(const std::string& name) {
	auto it = clips_.find(name);
	if (it == clips_.end()) {
		return nullptr;
	} // 取得失敗
	return &it->second; // 参照返し
}

const std::vector<MechAnimation::Pose>* MechAnimationContainer::GetFrames(const std::string& name) const {
	const MechAnimation::Clip* clip = GetClip(name);
	if (!clip) {
		return nullptr;
	} // 取得失敗
	return &clip->frames; // フレーム参照
}

std::vector<std::string> MechAnimationContainer::GetClipNames() const {
	std::vector<std::string> names;
	names.reserve(clips_.size()); // 予約

	for (const auto& kv : clips_) {
		names.push_back(kv.first); // 名前追加
	}

	return names; // 返却
}

bool MechAnimationContainer::SaveAllClips() const {
	const fs::path dirPath = fs::path(kDirectoryPath_);

	// ディレクトリ作成
	std::error_code ec;
	fs::create_directories(dirPath, ec);
	if (ec) {
		return false;
	}

	// 全クリップを書き出し
	for (const auto& kv : clips_) {
		const std::string& name = kv.first;
		const MechAnimation::Clip& clip = kv.second;

		// ファイル名を安全化
		const std::string safeName = SanitizeFileName(name);
		const fs::path filePath = dirPath / (safeName + ".json");

		// json生成
		const json j = ClipToJson(name, clip);

		// 書き出し
		std::ofstream ofs(filePath, std::ios::out | std::ios::trunc);
		if (!ofs) {
			return false;
		}
		ofs << j.dump(2);
	}

	return true;
}

bool MechAnimationContainer::LoadAllClips(bool overwrite) {
	const fs::path dirPath = fs::path(kDirectoryPath_);

	// ディレクトリが無ければ何もしない
	if (!fs::exists(dirPath)) {
		return false;
	}

	bool loadedAny = false;

	// ディレクトリ走査
	for (const auto& entry : fs::directory_iterator(dirPath)) {
		if (!entry.is_regular_file()) {
			continue;
		}

		const fs::path p = entry.path();
		if (p.extension() != ".json") {
			continue;
		}

		// 読み込み
		std::ifstream ifs(p);
		if (!ifs) {
			continue;
		}

		json j;
		try {
			ifs >> j;
		} catch (...) {
			continue;
		}

		// クリップ名
		std::string name;
		if (j.contains("name") && j["name"].is_string()) {
			name = j["name"].get<std::string>();
		} else {
			name = p.stem().string();
		}

		MechAnimation::Clip clip{};
		if (!JsonToClip(j, clip)) {
			continue;
		}

		// 追加
		if (AddClip(name, clip, overwrite)) {
			loadedAny = true;
		}
	}

	return loadedAny;
}
