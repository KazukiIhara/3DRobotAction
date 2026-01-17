#include "ParameterDataContainer.h"

#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <type_traits>
#include <algorithm>

#include "ImGuiController/ImGuiController.h"

using namespace Magi;

namespace {
	const std::filesystem::path kParamDir = "Assets/Parameters/";

	std::string ToString(ParamType t) {
		switch (t) {
			case ParamType::Int32:  return "Int32";
			case ParamType::Float:  return "Float";
			case ParamType::Vec2:   return "Vec2";
			case ParamType::Vec3:   return "Vec3";
			case ParamType::Vec4:   return "Vec4";
			case ParamType::String: return "String";
			default: return "Unknown";
		}
	}

	ParamType ToParamType(const std::string& s) {
		if (s == "Int32")  return ParamType::Int32;
		if (s == "Float")  return ParamType::Float;
		if (s == "Vec2")   return ParamType::Vec2;
		if (s == "Vec3")   return ParamType::Vec3;
		if (s == "Vec4")   return ParamType::Vec4;
		if (s == "String") return ParamType::String;
		throw std::runtime_error("ParameterDataContainer: Unknown ParamType string: " + s);
	}

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
			} else if constexpr (std::is_same_v<T, std::string>) {
				out["value"] = v;
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
			case ParamType::String:
				d.Value = v.get<std::string>();
				break;
			default:
				throw std::runtime_error("ParameterDataContainer: JsonToParamData unknown type");
		}
		return d;
	}

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

		const bool looksLikeValue = j.contains("type") && j.contains("value") && j.size() == 2;
		if (looksLikeValue) {
			node.value = JsonToParamData(j);
			return node;
		}

		for (auto it = j.begin(); it != j.end(); ++it) {
			node.children[it.key()] = JsonToNode(it.value());
		}
		return node;
	}

	ParamNode* GetOrCreateNode(ParamNode& root, const std::vector<std::string>& path, size_t startIndex) {
		ParamNode* cur = &root;
		for (size_t i = startIndex; i < path.size(); ++i) {
			const auto& key = path[i];
			cur = &cur->children[key];
		}
		return cur;
	}

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

	ParamNode* FindNodeMutable(ParamNode& root, const std::vector<std::string>& path, size_t startIndex) {
		ParamNode* cur = &root;
		for (size_t i = startIndex; i < path.size(); ++i) {
			const auto& key = path[i];
			auto it = cur->children.find(key);
			if (it == cur->children.end()) return nullptr;
			cur = &it->second;
		}
		return cur;
	}

#if defined(DEBUG) || defined(DEVELOP)
	const char* ToCString(ParamType t) {
		switch (t) {
			case ParamType::Int32:  return "Int32";
			case ParamType::Float:  return "Float";
			case ParamType::Vec2:   return "Vec2";
			case ParamType::Vec3:   return "Vec3";
			case ParamType::Vec4:   return "Vec4";
			case ParamType::String: return "String"; // 追加
			default: return "Unknown";
		}
	}

	void DrawParamValue(const ParamData& data) {
		ImGui::TextUnformatted(ToCString(data.Type));
		ImGui::SameLine();

		std::visit([&](auto&& v) {
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, int32_t>) {
				ImGui::Text(" %d", static_cast<int>(v));
			} else if constexpr (std::is_same_v<T, float>) {
				ImGui::Text(" %.6f", v);
			} else if constexpr (std::is_same_v<T, Vector2>) {
				ImGui::Text(" (%.3f, %.3f)", v.x, v.y);
			} else if constexpr (std::is_same_v<T, Vector3>) {
				ImGui::Text(" (%.3f, %.3f, %.3f)", v.x, v.y, v.z);
			} else if constexpr (std::is_same_v<T, Vector4>) {
				ImGui::Text(" (%.3f, %.3f, %.3f, %.3f)", v.x, v.y, v.z, v.w);
			} else if constexpr (std::is_same_v<T, std::string>) {
				ImGui::Text(" \"%s\"", v.c_str()); // 追加
			}
			}, data.Value);
	}

#endif
}

ParameterDataContainer::ParameterDataContainer() {
	LoadAllData();
}

ParameterDataContainer::~ParameterDataContainer() {
	SaveAllData();
}

void ParameterDataContainer::Update() {
#if defined(DEBUG) || defined(DEVELOP)
	// パラメータコンテナ用のデバッグウィンドウ
	if (!ImGui::Begin("ParameterDataContainer")) {
		ImGui::End();
		return;
	}

	// グループ名フィルタ文字列
	static char filter[128] = {};
	// 現在選択中のノードパス（Group / Tag / Key）
	static std::vector<std::string> selectedPath;

	//=========================
	// 追加UI
	//=========================
	ImGui::SeparatorText("Add");

	// "A/B/C" を {"A","B","C"} に分割
	auto splitPath = [](const char* s) -> std::vector<std::string> {
		std::vector<std::string> out;
		if (!s || s[0] == '\0') return out;

		std::string cur;
		for (const char* p = s; *p; ++p) {
			const char ch = *p;
			if (ch == '/' || ch == '\\') {
				if (!cur.empty()) {
					out.push_back(cur);
					cur.clear();
				}
			} else {
				cur.push_back(ch);
			}
		}
		if (!cur.empty()) out.push_back(cur);
		return out;
		};

	// グループ追加
	ImGui::TextUnformatted("Add Group");

	static char newGroup[128] = {};
	ImGui::InputText("GroupName", newGroup, IM_ARRAYSIZE(newGroup));
	ImGui::SameLine();
	if (ImGui::Button("AddGroup")) {
		if (newGroup[0] != '\0') {
			AddGroup(newGroup);
		}
	}

	ImGui::Separator();

	// 値追加
	ImGui::TextUnformatted("Add Value");

	// 既存グループ一覧（Combo用）
	static int groupIndex = 0;
	std::vector<std::string> groupNames;
	groupNames.reserve(paramDatas_.size());
	for (const auto& [k, _] : paramDatas_) {
		(void)_;
		groupNames.push_back(k);
	}
	std::sort(groupNames.begin(), groupNames.end());

	if (groupIndex >= static_cast<int>(groupNames.size())) {
		groupIndex = 0;
	}

	const char* currentGroup = groupNames.empty() ? "" : groupNames[groupIndex].c_str();

	if (groupNames.empty()) {
		ImGui::TextUnformatted("No groups. Add a group first.");
	} else {

		// グループ選択
		if (ImGui::BeginCombo("Group", currentGroup)) {
			for (int i = 0; i < static_cast<int>(groupNames.size()); ++i) {
				const bool selected = (i == groupIndex);
				if (ImGui::Selectable(groupNames[i].c_str(), selected)) {
					groupIndex = i;
				}
				if (selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		// タグパスとキー
		static char tagPath[256] = {};
		static char keyName[128] = {};
		ImGui::InputText("TagPath (A/B/C)", tagPath, IM_ARRAYSIZE(tagPath));
		ImGui::InputText("Key", keyName, IM_ARRAYSIZE(keyName));

		// 型選択
		static int typeIndex = 0;
		const char* typeItems[] = { "Int32", "Float", "Vec2", "Vec3", "Vec4", "String" };
		ImGui::Combo("Type", &typeIndex, typeItems, IM_ARRAYSIZE(typeItems));

		// 初期値
		static int   initI = 0;
		static float initF = 0.0f;
		static float init2[2] = { 0.0f, 0.0f };
		static float init3[3] = { 0.0f, 0.0f, 0.0f };
		static float init4[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static char  initS[256] = {}; // 追加

		switch (typeIndex) {
			case 0: ImGui::DragInt("Initial", &initI, 1.0f); break;
			case 1: ImGui::DragFloat("Initial", &initF, 0.01f); break;
			case 2: ImGui::DragFloat2("Initial", init2, 0.01f); break;
			case 3: ImGui::DragFloat3("Initial", init3, 0.01f); break;
			case 4: ImGui::DragFloat4("Initial", init4, 0.01f); break;
			case 5: ImGui::InputText("Initial", initS, IM_ARRAYSIZE(initS)); break;
			default: break;
		}

		// 追加実行
		if (ImGui::Button("AddValue")) {
			if (keyName[0] != '\0' && currentGroup[0] != '\0') {

				// Path構築：{Group, Tag..., Key}
				std::vector<std::string> path;
				path.reserve(16);
				path.push_back(currentGroup);

				const auto tags = splitPath(tagPath);
				for (const auto& t : tags) {
					path.push_back(t);
				}
				path.push_back(keyName);

				// ParamData構築
				ParamData d{};
				switch (typeIndex) {
					case 0:
						d.Type = ParamType::Int32;
						d.Value = static_cast<int32_t>(initI);
						break;
					case 1:
						d.Type = ParamType::Float;
						d.Value = initF;
						break;
					case 2:
						d.Type = ParamType::Vec2;
						d.Value = Vector2{ init2[0], init2[1] };
						break;
					case 3:
						d.Type = ParamType::Vec3;
						d.Value = Vector3{ init3[0], init3[1], init3[2] };
						break;
					case 4:
						d.Type = ParamType::Vec4;
						d.Value = Vector4{ init4[0], init4[1], init4[2], init4[3] };
						break;
					case 5:
						d.Type = ParamType::String;
						d.Value = std::string(initS); // 追加
						break;
					default:
						break;
				}

				AddData(path, d);
			}
		}

	}


	ImGui::Separator();

	//=========================
	// パラメータ一覧＋編集
	//=========================
	ImGui::SeparatorText("Parameters");

	if (ImGui::Button("SaveAll")) {
		SaveAllData();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload")) {
		LoadAllData();
	}

	// フィルタ入力
	ImGui::InputText("Filter", filter, IM_ARRAYSIZE(filter));
	ImGui::Separator();

	// 左右ペインの幅計算
	const float panelW = ImGui::GetContentRegionAvail().x;
	const float leftW = panelW * 0.55f;
	const float rightW = panelW - leftW;

	// -------------------------
	// 左：ツリー表示
	// -------------------------
	ImGui::BeginChild("Left", ImVec2(leftW, 0.0f), true);
	{
		ImGui::TextUnformatted("List");
		ImGui::Separator();

		std::vector<std::pair<std::string, ParamNode*>> groups;
		groups.reserve(paramDatas_.size());

		for (auto& [k, v] : paramDatas_) {
			if (filter[0] != '\0') {
				if (k.find(filter) == std::string::npos) continue;
			}
			groups.emplace_back(k, &v);
		}

		std::sort(groups.begin(), groups.end(),
			[](const auto& a, const auto& b) { return a.first < b.first; });

		std::function<void(const std::string&, ParamNode&, std::vector<std::string>&)> drawNode;
		drawNode = [&](const std::string& name, ParamNode& node, std::vector<std::string>& curPath) {
			ImGui::PushID(name.c_str());

			const bool hasValue = node.value.has_value();
			const bool hasChildren = !node.children.empty();
			const bool isSelected = (selectedPath == curPath);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
			if (hasValue || !hasChildren) {
				flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			}
			if (hasValue && isSelected) {
				flags |= ImGuiTreeNodeFlags_Selected;
			}

			const bool opened = ImGui::TreeNodeEx("##node", flags, "%s", name.c_str());

			if (hasValue && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
				selectedPath = curPath;
			}

			if (hasValue) {
				ImGui::SameLine();
				ImGui::TextUnformatted(" : ");
				ImGui::SameLine();
				ImGui::TextUnformatted(ToString(node.value->Type).c_str());
			}

			if (!hasValue && opened && hasChildren) {
				std::vector<std::pair<std::string, ParamNode*>> children;
				children.reserve(node.children.size());
				for (auto& [k, v] : node.children) {
					children.emplace_back(k, &v);
				}
				std::sort(children.begin(), children.end(),
					[](const auto& a, const auto& b) { return a.first < b.first; });

				for (auto& [childName, childPtr] : children) {
					curPath.push_back(childName);
					drawNode(childName, *childPtr, curPath);
					curPath.pop_back();
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
			};

		for (auto& [groupName, rootPtr] : groups) {
			ImGui::PushID(groupName.c_str());

			if (ImGui::TreeNodeEx("##group", ImGuiTreeNodeFlags_SpanAvailWidth, "%s", groupName.c_str())) {

				std::vector<std::pair<std::string, ParamNode*>> children;
				children.reserve(rootPtr->children.size());
				for (auto& [k, v] : rootPtr->children) {
					children.emplace_back(k, &v);
				}
				std::sort(children.begin(), children.end(),
					[](const auto& a, const auto& b) { return a.first < b.first; });

				for (auto& [childName, childPtr] : children) {
					std::vector<std::string> curPath;
					curPath.reserve(16);
					curPath.push_back(groupName);
					curPath.push_back(childName);

					drawNode(childName, *childPtr, curPath);
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		ImGui::EndChild();

		ImGui::SameLine();

		// -------------------------
		// 右：値編集
		// -------------------------
		ImGui::BeginChild("Right", ImVec2(rightW, 0.0f), true);
		{
			ImGui::TextUnformatted("Edit");
			ImGui::Separator();

			bool canEdit = true;

			if (selectedPath.size() < 2) {
				ImGui::TextUnformatted("Select a value node.");
				canEdit = false;
			}

			ParamNode* node = nullptr;
			if (canEdit) {
				const std::string& groupName = selectedPath[0];
				auto git = paramDatas_.find(groupName);
				if (git == paramDatas_.end()) {
					ImGui::TextUnformatted("Group not found.");
					canEdit = false;
				} else {
					ParamNode& groupRoot = git->second;
					node = FindNodeMutable(groupRoot, selectedPath, 1);
					if (!node || !node->value.has_value()) {
						ImGui::TextUnformatted("Node not found.");
						canEdit = false;
					}
				}
			}

			if (canEdit) {
				ParamData& data = *node->value;

				// 選択中パス表示
				ImGui::Text("Path:");
				for (size_t i = 0; i < selectedPath.size(); ++i) {
					ImGui::SameLine(0.0f, 0.0f);
					ImGui::Text("%s%s", (i == 0 ? "" : "/"), selectedPath[i].c_str());
				}

				// 型表示
				ImGui::Text("Type: %s", ToString(data.Type).c_str());
				ImGui::Separator();

				// 型ごとに Drag 操作で値編集
				switch (data.Type) {
					case ParamType::Int32: {
						int v = std::get<int32_t>(data.Value);
						if (ImGui::DragInt("Value", &v, 1.0f)) {
							data.Value = static_cast<int32_t>(v);
						}
					} break;

					case ParamType::Float: {
						float v = std::get<float>(data.Value);
						if (ImGui::DragFloat("Value", &v, 0.01f)) {
							data.Value = v;
						}
					} break;

					case ParamType::Vec2: {
						Vector2 v = std::get<Vector2>(data.Value);
						float arr[2] = { v.x, v.y };
						if (ImGui::DragFloat2("Value", arr, 0.01f)) {
							v.x = arr[0];
							v.y = arr[1];
							data.Value = v;
						}
					} break;

					case ParamType::Vec3: {
						Vector3 v = std::get<Vector3>(data.Value);
						float arr[3] = { v.x, v.y, v.z };
						if (ImGui::DragFloat3("Value", arr, 0.01f)) {
							v.x = arr[0];
							v.y = arr[1];
							v.z = arr[2];
							data.Value = v;
						}
					} break;

					case ParamType::Vec4: {
						Vector4 v = std::get<Vector4>(data.Value);
						float arr[4] = { v.x, v.y, v.z, v.w };
						if (ImGui::DragFloat4("Value", arr, 0.01f)) {
							v.x = arr[0];
							v.y = arr[1];
							v.z = arr[2];
							v.w = arr[3];
							data.Value = v;
						}
					} break;

					case ParamType::String: { // 追加
						std::string v = std::get<std::string>(data.Value);
						static char buf[512]{};

						std::snprintf(buf, sizeof(buf), "%s", v.c_str());
						if (ImGui::InputText("Value", buf, IM_ARRAYSIZE(buf))) {
							data.Value = std::string(buf);
						}
					} break;

					default:
						ImGui::TextUnformatted("Unsupported type.");
						break;
				}

				ImGui::Separator();

				// 削除ボタン
				ImGui::Spacing();
				ImGui::Separator();

				if (ImGui::Button("Delete")) {
					ImGui::OpenPopup("Confirm Delete");
				}

				// 確認ダイアログ
				if (ImGui::BeginPopupModal("Confirm Delete", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

					ImGui::TextUnformatted("Delete this parameter?");
					ImGui::Separator();

					// 削除対象パス表示
					ImGui::TextUnformatted("Path:");
					for (size_t i = 0; i < selectedPath.size(); ++i) {
						ImGui::SameLine(0.0f, 0.0f);
						ImGui::Text("%s%s", (i == 0 ? "" : "/"), selectedPath[i].c_str());
					}

					ImGui::Spacing();

					// 実行
					if (ImGui::Button("OK")) {
						DeleteData(selectedPath);
						selectedPath.clear();
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					// キャンセル
					if (ImGui::Button("Cancel")) {
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

		}
		ImGui::EndChild();
	}


	ImGui::End();
#endif
}

void ParameterDataContainer::LoadAllData() {
	paramDatas_.clear();

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
		} catch (...) {
			continue;
		}

		const std::string groupName = entry.path().stem().string();

		if (j.is_object()) {
			paramDatas_[groupName] = JsonToNode(j);
		} else {
			paramDatas_[groupName] = ParamNode{};
		}
	}
}

void ParameterDataContainer::SaveAllData() {
	std::filesystem::create_directories(kParamDir);

	std::unordered_set<std::string> groupSet;
	groupSet.reserve(paramDatas_.size());
	for (const auto& [groupName, _] : paramDatas_) {
		(void)_;
		groupSet.insert(groupName);
	}

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

	for (const auto& [groupName, root] : paramDatas_) {
		nlohmann::json j = NodeToJson(root);
		const auto outPath = kParamDir / (groupName + ".json");

		std::ofstream ofs(outPath, std::ios::out | std::ios::trunc);
		if (!ofs) {
			throw std::runtime_error("ParameterDataContainer: failed to open file for write: " + outPath.string());
		}
		ofs << j.dump(2);
	}
}

void ParameterDataContainer::AddGroup(const std::string& groupName) {
	if (paramDatas_.find(groupName) == paramDatas_.end()) {
		paramDatas_.emplace(groupName, ParamNode{});
	}
}

void ParameterDataContainer::AddTag(const std::vector<std::string>& path) {
	if (path.size() < 2) {
		throw std::runtime_error("ParameterDataContainer::AddTag path must be [group, tag...]");
	}

	const std::string& groupName = path[0];
	AddGroup(groupName);

	ParamNode& groupRoot = paramDatas_.at(groupName);

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

	if (const ParamNode* existing = FindNode(groupRoot, path, 1)) {
		if (existing->value.has_value()) {
			return;
		}
	}

	ParamNode* node = GetOrCreateNode(groupRoot, path, 1);

	if (!node->children.empty()) {
		throw std::runtime_error("ParameterDataContainer::AddData node already has children");
	}

	node->value = data;
}

void ParameterDataContainer::AddData(const std::vector<std::string>& path, const ParamType& type) {
	ParamData data{};
	data.Type = type;

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
		case ParamType::String:
			data.Value = std::string{};
			break;
		default:
			throw std::runtime_error("ParameterDataContainer::AddData unknown ParamType");
			break;
	}

	AddData(path, data);
}

void ParameterDataContainer::DeleteData(const std::vector<std::string>& path) {
	if (path.size() < 2) {
		throw std::runtime_error("ParameterDataContainer::DeleteData path must be [group, ..., key]");
	}

	const std::string& groupName = path[0];
	auto git = paramDatas_.find(groupName);
	if (git == paramDatas_.end()) {
		return; // グループが無ければ何もしない
	}

	ParamNode& root = git->second;

	// 再帰削除用ラムダ
	std::function<bool(ParamNode&, size_t)> eraseRecursive;
	eraseRecursive = [&](ParamNode& node, size_t index) -> bool {

		const std::string& key = path[index];
		auto it = node.children.find(key);
		if (it == node.children.end()) {
			return false;
		}

		// 最終ノード
		if (index + 1 == path.size()) {
			it->second.value.reset();
		} else {
			// 子へ進む
			if (!eraseRecursive(it->second, index + 1)) {
				return false;
			}
		}

		// 子が空になったら削除
		if (!it->second.value.has_value() && it->second.children.empty()) {
			node.children.erase(it);
		}

		return true;
		};

	eraseRecursive(root, 1);
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