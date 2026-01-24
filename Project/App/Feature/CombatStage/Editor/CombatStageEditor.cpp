#include "CombatStageEditor.h"

#include "Feature/CombatStage/Data/CombatStageData.h"

// C++
#include <algorithm>
#include <cstdio>

// ImGui
#include "imgui/imgui.h"

CombatStageEditor::CombatStageEditor(CombatStageData* stageData) {
	stageData_ = stageData;

	// パス初期化
	std::snprintf(pathBuf_, sizeof(pathBuf_), "%s", path_.c_str());
}

void CombatStageEditor::SetPath(const std::string& path) {
	path_ = path;

	// パス更新
	std::snprintf(pathBuf_, sizeof(pathBuf_), "%s", path_.c_str());
}

void CombatStageEditor::DrawImGui() {
	if (!stageData_) return;

	DrawToolbar();
	ImGui::Separator();

	ImGui::Columns(2, "StageColliderEditorCols", true);
	DrawList();
	ImGui::NextColumn();
	DrawInspector();
	ImGui::Columns(1);
}

void CombatStageEditor::DrawToolbar() {
	ImGui::TextUnformatted("Stage Collider Editor (AABB)");

	// 保存パス
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::InputText("##Path", pathBuf_, sizeof(pathBuf_));

	// 追加
	if (ImGui::Button("Add AABB")) {
		AddAABB();
	}
	ImGui::SameLine();

	// 複製
	if (ImGui::Button("Duplicate") && selected_ >= 0) {
		Duplicate();
	}
	ImGui::SameLine();

	// 削除
	if (ImGui::Button("Remove") && selected_ >= 0) {
		Remove();
	}

	// 保存/読込
	if (ImGui::Button("Save JSON")) {
		stageData_->SaveColliderJson(std::string(pathBuf_));
	}
	ImGui::SameLine();
	if (ImGui::Button("Load JSON")) {
		if (stageData_->LoadColliderJson(std::string(pathBuf_))) {
			const auto& list = stageData_->GetCollider();
			selected_ = list.empty() ? -1 : 0;
		}
	}
}

void CombatStageEditor::DrawList() {
	ImGui::TextUnformatted("AABB List");

	auto& list = stageData_->GetColliderMutable();

	for (int i = 0; i < static_cast<int>(list.size()); ++i) {
		auto& c = list[i];

		ImGui::PushID(i);

		// 有効
		ImGui::Checkbox("##Active", &c.isActive_);
		ImGui::SameLine();

		// 選択
		const bool isSel = (selected_ == i);
		const std::string label = c.name.empty() ? ("AABB_" + std::to_string(i)) : c.name;
		if (ImGui::Selectable(label.c_str(), isSel)) {
			selected_ = i;
		}

		ImGui::PopID();
	}
}

void CombatStageEditor::DrawInspector() {
	ImGui::TextUnformatted("Inspector");

	auto& list = stageData_->GetColliderMutable();
	if (selected_ < 0 || selected_ >= static_cast<int>(list.size())) {
		ImGui::TextUnformatted("No selection.");
		return;
	}

	auto& c = list[selected_];

	// 名前
	{
		char buf[256]{};
		std::snprintf(buf, sizeof(buf), "%s", c.name.c_str());
		if (ImGui::InputText("Name", buf, sizeof(buf))) {
			c.name = buf;
		}
	}

	// min/max
	ImGui::DragFloat3("Min", &c.min.x, 0.05f);
	ImGui::DragFloat3("Max", &c.max.x, 0.05f);

	// min <= max にする
	c.min.x = std::min(c.min.x, c.max.x);
	c.min.y = std::min(c.min.y, c.max.y);
	c.min.z = std::min(c.min.z, c.max.z);

	c.max.x = std::max(c.min.x, c.max.x);
	c.max.y = std::max(c.min.y, c.max.y);
	c.max.z = std::max(c.min.z, c.max.z);

	// クイック
	if (ImGui::Button("Set Unit (0..1)")) {
		c.min = { 0.0f,0.0f,0.0f };
		c.max = { 1.0f,1.0f,1.0f };
	}
}

void CombatStageEditor::AddAABB() {
	auto& list = stageData_->GetColliderMutable();

	CombatStage::AABB c{};
	// 初期化
	c.min = { 0.0f,0.0f,0.0f };
	c.max = { 1.0f,1.0f,1.0f };
	c.isActive_ = true;

	// 名前
	c.name = "AABB_" + std::to_string(static_cast<int>(list.size()));

	list.push_back(c);
	selected_ = static_cast<int>(list.size()) - 1;
}

void CombatStageEditor::Duplicate() {
	auto& list = stageData_->GetColliderMutable();
	if (selected_ < 0 || selected_ >= static_cast<int>(list.size())) return;

	CombatStage::AABB copy = list[selected_];
	// 名前
	copy.name += "_Copy";

	list.push_back(copy);
	selected_ = static_cast<int>(list.size()) - 1;
}

void CombatStageEditor::Remove() {
	auto& list = stageData_->GetColliderMutable();
	if (selected_ < 0 || selected_ >= static_cast<int>(list.size())) return;

	list.erase(list.begin() + selected_);

	// 選択補正
	if (list.empty()) {
		selected_ = -1;
	} else {
		selected_ = std::min(selected_, static_cast<int>(list.size()) - 1);
	}
}
