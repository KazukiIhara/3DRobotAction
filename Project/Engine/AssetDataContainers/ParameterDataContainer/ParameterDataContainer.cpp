#include "ParameterDataContainer.h"

#include <fstream>
#include <stdexcept>
#include <unordered_set>

using namespace Magi;

namespace {
	const std::filesystem::path kParamDir = "Assets/Parameters/";

	// ParamType <-> string
	std::string ToString(ParamType t) {
		switch (t) {
		case ParamType::Int32: return "Int32";
		case ParamType::Float: return "Float";
		case ParamType::Vec2:  return "Vec2";
		case ParamType::Vec3:  return "Vec3";
		case ParamType::Vec4:  return "Vec4";
		default: return "Unknown";
		}
	}

	ParamType ToParamType(const std::string& s) {
		if (s == "Int32") return ParamType::Int32;
		if (s == "Float") return ParamType::Float;
		if (s == "Vec2")  return ParamType::Vec2;
		if (s == "Vec3")  return ParamType::Vec3;
		if (s == "Vec4")  return ParamType::Vec4;
		throw std::runtime_error("ParameterDataContainer: Unknown ParamType string: " + s);
	}

	// Vector <-> json
	nlohmann::json VecToJson(const Vector2& v) {
		return nlohmann::json{ {"x", v.x}, {"y", v.y} };
	}
	nlohmann::json VecToJson(const Vector3& v) {
		return nlohmann::json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
	}
	nlohmann::json VecToJson(const Vector4& v) {
		return nlohmann::json{ {"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w} };
	}

	Vector2 JsonToVec2(const nlohmann::json& j) {
		Vector2 v{};
		v.x = j.at("x").get<float>();
		v.y = j.at("y").get<float>();
		return v;
	}
	Vector3 JsonToVec3(const nlohmann::json& j) {
		Vector3 v{};
		v.x = j.at("x").get<float>();
		v.y = j.at("y").get<float>();
		v.z = j.at("z").get<float>();
		return v;
	}
	Vector4 JsonToVec4(const nlohmann::json& j) {
		Vector4 v{};
		v.x = j.at("x").get<float>();
		v.y = j.at("y").get<float>();
		v.z = j.at("z").get<float>();
		v.w = j.at("w").get<float>();
		return v;
	}

	// ParamData <-> json
	nlohmann::json ParamDataToJson(const ParamData& d) {
		nlohmann::json out;
		out["type"] = ToString(d.Type);

		std::visit([&](auto&& v) {
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, int32_t>) {
				out["value"] = v;
			} else if constexpr (std::is_same_v<T, float>) {
				out["value"] = v;
			} else if constexpr (std::is_same_v<T, Vector2>) {
				out["value"] = VecToJson(v);
			} else if constexpr (std::is_same_v<T, Vector3>) {
				out["value"] = VecToJson(v);
			} else if constexpr (std::is_same_v<T, Vector4>) {
				out["value"] = VecToJson(v);
			} else {
				static_assert(sizeof(T) == 0, "Unsupported ParamValue type");
			}
			}, d.Value);

		return out;
	}

	ParamData JsonToParamData(const nlohmann::json& j) {
		ParamData d{};
		d.Type = ToParamType(j.at("type").get<std::string>());

		const auto& v = j.at("value");
		switch (d.Type) {
		case ParamType::Int32:
			d.Value = v.get<int32_t>();
			break;
		case ParamType::Float:
			d.Value = v.get<float>();
			break;
		case ParamType::Vec2:
			d.Value = JsonToVec2(v);
			break;
		case ParamType::Vec3:
			d.Value = JsonToVec3(v);
			break;
		case ParamType::Vec4:
			d.Value = JsonToVec4(v);
			break;
		default:
			throw std::runtime_error("ParameterDataContainer: JsonToParamData unknown type");
		}
		return d;
	}

	// ParamNode <-> json（再帰）
	nlohmann::json NodeToJson(const ParamNode& node) {
		if (node.value.has_value()) {
			if (!node.children.empty()) {
				throw std::runtime_error("ParameterDataContainer: node has both children and value");
			}
			return ParamDataToJson(*node.value);
		}

		nlohmann::json j = nlohmann::json::object();
		for (const auto& [k, child] : node.children) {
			j[k] = NodeToJson(child);
		}
		return j;
	}

	ParamNode JsonToNode(const nlohmann::json& j) {
		ParamNode node{};

		if (!j.is_object()) {
			throw std::runtime_error("ParameterDataContainer: invalid json node (must be object)");
		}

		const bool looksLikeValue =
			j.contains("type") && j.contains("value") && j.size() == 2;

		if (looksLikeValue) {
			node.value = JsonToParamData(j);
			return node;
		}

		for (auto it = j.begin(); it != j.end(); ++it) {
			node.children[it.key()] = JsonToNode(it.value());
		}
		return node;
	}

	// 指定パスのノードを作成しながら取得（startIndex から末尾まで）
	ParamNode* GetOrCreateNode(ParamNode& root, const std::vector<std::string>& path, size_t startIndex) {
		ParamNode* cur = &root;
		for (size_t i = startIndex; i < path.size(); ++i) {
			const auto& key = path[i];
			cur = &cur->children[key];
		}
		return cur;
	}

	// 指定パスのノードを検索（存在しない場合 nullptr）
	const ParamNode* FindNode(const ParamNode& root, const std::vector<std::string>& path, size_t startIndex) {
		const ParamNode* cur = &root;
		for (size_t i = startIndex; i < path.size(); ++i) {
			const auto& key = path[i];
			auto it = cur->children.find(key);
			if (it == cur->children.end()) return nullptr;
			cur = &it->second;
		}
		return cur;
	}
}

ParameterDataContainer::ParameterDataContainer() {
	// データをすべてロード
	LoadAllData();
}

ParameterDataContainer::~ParameterDataContainer() {
	// データをすべてセーブ
	SaveAllData();
}

void ParameterDataContainer::LoadAllData() {
	paramDatas_.clear();
	groups_.clear();

	if (!std::filesystem::exists(kParamDir)) {
		return;
	}

	for (const auto& entry : std::filesystem::directory_iterator(kParamDir)) {
		if (!entry.is_regular_file()) continue;
		if (entry.path().extension() != ".json") continue;

		std::ifstream ifs(entry.path());
		if (!ifs) continue;

		nlohmann::json j;
		try {
			ifs >> j;
		}
		catch (...) {
			continue;
		}

		const std::string groupName = entry.path().stem().string();
		groups_.push_back({ groupName, false });

		if (j.is_object()) {
			paramDatas_[groupName] = JsonToNode(j);
		} else {
			paramDatas_[groupName] = ParamNode{};
		}
	}
}

void ParameterDataContainer::SaveAllData() {
	std::filesystem::create_directories(kParamDir);

	// 保存対象グループ一覧
	std::unordered_set<std::string> groupSet;
	groupSet.reserve(groups_.size());
	for (const auto& [groupName, debugFlag] : groups_) {
		(void)debugFlag;
		groupSet.insert(groupName);
	}

	// ディスク上にある json で、groupSet に無いものは削除
	if (std::filesystem::exists(kParamDir)) {
		for (const auto& entry : std::filesystem::directory_iterator(kParamDir)) {
			if (!entry.is_regular_file()) continue;
			if (entry.path().extension() != ".json") continue;

			const std::string fileGroup = entry.path().stem().string();
			if (groupSet.find(fileGroup) == groupSet.end()) {
				std::error_code ec;
				std::filesystem::remove(entry.path(), ec);
			}
		}
	}

	// groups_ にあるものだけ保存（毎回全書き込み）
	for (const auto& [groupName, debugFlag] : groups_) {
		(void)debugFlag;

		auto it = paramDatas_.find(groupName);
		if (it == paramDatas_.end()) {
			it = paramDatas_.emplace(groupName, ParamNode{}).first;
		}

		nlohmann::json j = NodeToJson(it->second);
		const auto outPath = kParamDir / (groupName + ".json");

		std::ofstream ofs(outPath, std::ios::out | std::ios::trunc);
		if (!ofs) {
			throw std::runtime_error("ParameterDataContainer: failed to open file for write: " + outPath.string());
		}
		ofs << j.dump(2);
	}
}

void ParameterDataContainer::AddGroup(const std::string& groupName) {
	bool exists = false;
	for (const auto& g : groups_) {
		if (g.first == groupName) {
			exists = true;
			break;
		}
	}
	if (!exists) {
		groups_.push_back({ groupName, false });
	}

	if (paramDatas_.find(groupName) == paramDatas_.end()) {
		paramDatas_.emplace(groupName, ParamNode{});
	}
}

void ParameterDataContainer::AddTag(const std::vector<std::string>& path) {
	if (path.empty()) {
		throw std::runtime_error("ParameterDataContainer::AddTag path is empty");
	}

	const std::string& groupName = path[0];
	AddGroup(groupName);

	ParamNode& groupRoot = paramDatas_.at(groupName);

	// 既に同名タグがあるなら何もしない
	if (const ParamNode* node = FindNode(groupRoot, path, 1)) {
		(void)node;
		return;
	}

	(void)GetOrCreateNode(groupRoot, path, 1);
}

void ParameterDataContainer::AddData(const std::vector<std::string>& path, const ParamData& data) {
	if (path.size() < 2) {
		throw std::runtime_error("ParameterDataContainer::AddData path must be [group, ..., key]");
	}

	const std::string& groupName = path[0];
	AddGroup(groupName);

	ParamNode& groupRoot = paramDatas_.at(groupName);

	// 既に同名データがあり、値が入っているならスキップ
	if (const ParamNode* existing = FindNode(groupRoot, path, 1)) {
		if (existing->value.has_value()) {
			return;
		}
	}

	ParamNode* node = GetOrCreateNode(groupRoot, path, 1);

	// 子を持つノードに値を入れない
	if (!node->children.empty()) {
		throw std::runtime_error("ParameterDataContainer::AddData node already has children");
	}

	node->value = data;
}

void ParameterDataContainer::AddData(const std::vector<std::string>& path, const ParamType& type) {
	ParamData data{};
	data.Type = type;

	// 型に合わせて初期化
	switch (type) {
	case ParamType::Int32:
		data.Value = int32_t{ 0 };
		break;
	case ParamType::Float:
		data.Value = 0.0f;
		break;
	case ParamType::Vec2:
		data.Value = Vector2{ 0.0f, 0.0f };
		break;
	case ParamType::Vec3:
		data.Value = Vector3{ 0.0f, 0.0f, 0.0f };
		break;
	case ParamType::Vec4:
		data.Value = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
		break;
	default:
		throw std::runtime_error("ParameterDataContainer::AddData unknown ParamType");
	}

	// 既存のAddData
	AddData(path, data);
}

ParamValue ParameterDataContainer::GetValue(const std::vector<std::string>& path) {
	if (path.size() < 2) {
		throw std::runtime_error("ParameterDataContainer::GetValue path must be [group, ..., key]");
	}

	const std::string& groupName = path[0];
	auto git = paramDatas_.find(groupName);
	if (git == paramDatas_.end()) {
		throw std::runtime_error("ParameterDataContainer::GetValue group not found: " + groupName);
	}

	const ParamNode& groupRoot = git->second;
	const ParamNode* node = FindNode(groupRoot, path, 1);
	if (!node) {
		throw std::runtime_error("ParameterDataContainer::GetValue path not found");
	}
	if (!node->value.has_value()) {
		throw std::runtime_error("ParameterDataContainer::GetValue node has no value");
	}

	return node->value->Value;
}
