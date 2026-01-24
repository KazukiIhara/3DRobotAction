#include "CombatStageData.h"

// C++
#include <fstream>

// JSON
#include <nlohmann/json.hpp>

#include "MAGI.h"

using namespace Magi;

using json = nlohmann::json;

namespace {

	json Vec3ToJson(const Vector3& v) {
		return json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
	}

	Vector3 JsonToVec3(const json& j, const Vector3& fallback) {
		Vector3 v = fallback;
		if (!j.is_object()) return v;
		if (j.contains("x")) v.x = j["x"].get<float>();
		if (j.contains("y")) v.y = j["y"].get<float>();
		if (j.contains("z")) v.z = j["z"].get<float>();
		return v;
	}

}

CombatStageData::CombatStageData() {

}

void CombatStageData::Draw() {

#if defined(DEBUG)|(DEVELOP)
	if (debugFlag_.showImGui) {
		DrawImGui();
	}
	if (debugFlag_.isDrawCollider) {
		for (auto& collider : colliders_) {
			if (collider.isActive_) {
				MAGISYSTEM::DrawLineAABB(collider.min, collider.max, Color::Green);
			}
		}
	}
#endif	


}

const std::vector<CombatStage::AABB>& CombatStageData::GetCollider() const {
	return colliders_;
}

std::vector<CombatStage::AABB>& CombatStageData::GetColliderMutable() {
	return colliders_;
}

void CombatStageData::AddCollider(const CombatStage::AABB& aabb) {
	// 追加
	colliders_.push_back(aabb);
}

void CombatStageData::ClearCollider() {
	// 全消し
	colliders_.clear();
}

bool CombatStageData::SaveColliderJson(const std::string& path) const {
	json root{};
	// バージョン
	root["version"] = 1;

	json arr = json::array();
	for (const auto& c : colliders_) {
		json item{};
		// 名前
		item["name"] = c.name;
		// 有効
		item["active"] = c.isActive_;
		// AABB
		item["min"] = Vec3ToJson(c.min);
		item["max"] = Vec3ToJson(c.max);
		arr.push_back(item);
	}
	root["aabbs"] = arr;

	std::ofstream ofs(path);
	if (!ofs.is_open()) return false;

	ofs << root.dump(2);
	return true;
}

bool CombatStageData::LoadColliderJson(const std::string& path) {
	std::ifstream ifs(path);
	if (!ifs.is_open()) return false;

	json root{};
	ifs >> root;

	if (!root.contains("aabbs") || !root["aabbs"].is_array()) return false;

	std::vector<CombatStage::AABB> loaded{};
	for (const auto& item : root["aabbs"]) {
		CombatStage::AABB c{};

		// 名前
		if (item.contains("name")) c.name = item["name"].get<std::string>();
		// 有効
		c.isActive_ = item.value("active", true);

		// min/max
		c.min = JsonToVec3(item.value("min", json{}), { 0.0f,0.0f,0.0f });
		c.max = JsonToVec3(item.value("max", json{}), { 1.0f,1.0f,1.0f });

		loaded.push_back(c);
	}

	colliders_ = std::move(loaded);
	return true;
}

const CombatStageData::DebugFlag& CombatStageData::GetDebugFlag()const {
	return debugFlag_;
}

void CombatStageData::SwitchShowImGui() {
	debugFlag_.showImGui = !debugFlag_.showImGui;
}

void CombatStageData::DrawImGui() {
	ImGui::Begin("Stage");
	ImGui::SeparatorText("DebugFlag");
	if (ImGui::Button("SwitchDrawCollider")) {
		SwitchIsDrawCollider();
	}
	ImGui::End();
}

void CombatStageData::SwitchIsDrawCollider() {
	debugFlag_.isDrawCollider = !debugFlag_.isDrawCollider;
}