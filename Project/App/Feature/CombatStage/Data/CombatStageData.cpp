#include "CombatStageData.h"

// C++
#include <fstream>

// JSON
#include <nlohmann/json.hpp>

// ImGui
#include "imgui/imgui.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"

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

#if defined(DEBUG) || defined(DEVELOP)
	if (debugFlag_.showImGui) {
		DrawImGui();
	}

	// コライダー描画
	if (debugFlag_.isDrawCollider) {
		for (auto& collider : colliders_) {
			if (collider.isActive_) {
				MAGISYSTEM::DrawLineAABB(collider.min, collider.max, Color::Green);
			}
		}
	}
#endif

	// モデル描画（常に描画）
	for (auto& m : models_) {
		if (!m.isActive_) continue;
		if (m.modelName.empty()) continue;

		// 行列更新
		UpdateModelWorldMatrix(m);

		// 描画
		MAGISYSTEM::DrawModel(m.modelName, m.worldMatrix);
	}
}

const std::vector<CombatStage::AABB>& CombatStageData::GetCollider() const {
	return colliders_;
}

std::vector<CombatStage::AABB>& CombatStageData::GetColliderMutable() {
	return colliders_;
}

const std::vector<CombatStageData::StageModelData>& CombatStageData::GetModels() const {
	return models_;
}

std::vector<CombatStageData::StageModelData>& CombatStageData::GetModelsMutable() {
	return models_;
}

void CombatStageData::AddCollider(const CombatStage::AABB& aabb) {
	// 追加
	colliders_.push_back(aabb);
}

void CombatStageData::ClearCollider() {
	// 全消し
	colliders_.clear();
}

void CombatStageData::AddModel(const StageModelData& model) {
	// 追加
	models_.push_back(model);
	// 行列更新
	UpdateModelWorldMatrix(models_.back());
}

void CombatStageData::ClearModels() {
	// 全消し
	models_.clear();
}

void CombatStageData::UpdateModelWorldMatrix(StageModelData& model) {
	// ワールド行列更新
	model.worldMatrix = MAGIMath::MakeAffineMatrix(model.scale, model.rotate, model.translate);
}

bool CombatStageData::SaveJson(const std::string& path) const {
	json root{};
	// バージョン
	root["version"] = 2;

	// AABB
	json aabbArr = json::array();
	for (const auto& c : colliders_) {
		json item{};
		// 名前
		item["name"] = c.name;
		// 有効
		item["active"] = c.isActive_;
		// AABB
		item["min"] = Vec3ToJson(c.min);
		item["max"] = Vec3ToJson(c.max);
		aabbArr.push_back(item);
	}
	root["aabbs"] = aabbArr;

	// Models
	json modelArr = json::array();
	for (const auto& m : models_) {
		json item{};
		// 管理名
		item["name"] = m.name;
		// モデル名
		item["modelName"] = m.modelName;
		// 有効
		item["active"] = m.isActive_;
		// TRS
		item["translate"] = Vec3ToJson(m.translate);
		item["rotate"] = Vec3ToJson(m.rotate);
		item["scale"] = Vec3ToJson(m.scale);
		modelArr.push_back(item);
	}
	root["models"] = modelArr;

	std::ofstream ofs(path);
	if (!ofs.is_open()) return false;

	ofs << root.dump(2);
	return true;
}

bool CombatStageData::LoadJson(const std::string& path) {
	std::ifstream ifs(path);
	if (!ifs.is_open()) return false;

	json root{};
	ifs >> root;

	// AABB
	std::vector<CombatStage::AABB> loadedAabbs{};
	if (root.contains("aabbs") && root["aabbs"].is_array()) {
		for (const auto& item : root["aabbs"]) {
			CombatStage::AABB c{};

			// 名前
			if (item.contains("name")) c.name = item["name"].get<std::string>();
			// 有効
			c.isActive_ = item.value("active", true);

			// min/max
			c.min = JsonToVec3(item.value("min", json{}), { 0.0f,0.0f,0.0f });
			c.max = JsonToVec3(item.value("max", json{}), { 1.0f,1.0f,1.0f });

			loadedAabbs.push_back(c);
		}
	}

	// Models
	std::vector<StageModelData> loadedModels{};
	if (root.contains("models") && root["models"].is_array()) {
		for (const auto& item : root["models"]) {
			StageModelData m{};

			// 管理名
			if (item.contains("name")) m.name = item["name"].get<std::string>();
			// モデル名
			if (item.contains("modelName")) m.modelName = item["modelName"].get<std::string>();
					
			// モデルをロード
			MAGISYSTEM::LoadCreateModel(m.modelName);
			
			// 有効
			m.isActive_ = item.value("active", true);

			// TRS
			m.translate = JsonToVec3(item.value("translate", json{}), { 0.0f,0.0f,0.0f });
			m.rotate = JsonToVec3(item.value("rotate", json{}), { 0.0f,0.0f,0.0f });
			m.scale = JsonToVec3(item.value("scale", json{}), { 1.0f,1.0f,1.0f });

			// 行列更新
			UpdateModelWorldMatrix(m);

			loadedModels.push_back(m);
		}
	}

	colliders_ = std::move(loadedAabbs);
	models_ = std::move(loadedModels);
	return true;
}

const CombatStageData::DebugFlag& CombatStageData::GetDebugFlag() const {
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
