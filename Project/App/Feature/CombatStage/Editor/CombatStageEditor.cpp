#include "CombatStageEditor.h"

#include "Feature/CombatStage/Data/CombatStageData.h"

// C++
#include <algorithm>
#include <cstdio>

// ImGui
#include "imgui/imgui.h"

#include "MAGI.h"

using namespace Magi;

CombatStageEditor::CombatStageEditor(CombatStageData* stageData) {
	stageData_ = stageData;

	std::snprintf(pathBuf_, sizeof(pathBuf_), "%s", path_.c_str());

	// 追加用の初期値
	std::snprintf(newModelNameBuf_, sizeof(newModelNameBuf_), "%s", "Model_0");
	std::snprintf(newModelModelNameBuf_, sizeof(newModelModelNameBuf_), "%s", "Default");
}


void CombatStageEditor::SetPath(const std::string& path) {
	path_ = path;

	// パス更新
	std::snprintf(pathBuf_, sizeof(pathBuf_), "%s", path_.c_str());
}

void CombatStageEditor::DrawImGui() {
	if (!stageData_) return;

	DrawToolbar();
	DrawModeSwitch();

	ImGui::Separator();
	ImGui::Columns(2, "StageEditorCols", true);

	if (mode_ == EditMode::Collider) {
		DrawListCollider();
		ImGui::NextColumn();
		DrawInspectorCollider();
	} else {
		DrawListModel();
		ImGui::NextColumn();
		DrawInspectorModel();
	}

	ImGui::Columns(1);
}

void CombatStageEditor::DrawToolbar() {
	ImGui::TextUnformatted("Stage Editor");

	// 保存パス
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::InputText("##Path", pathBuf_, sizeof(pathBuf_));

	// 保存/読込
	if (ImGui::Button("Save JSON")) {
		stageData_->SaveJson(std::string(pathBuf_));
	}
	ImGui::SameLine();
	if (ImGui::Button("Load JSON")) {
		if (stageData_->LoadJson(std::string(pathBuf_))) {
			selectedCollider_ = stageData_->GetCollider().empty() ? -1 : 0;
			selectedModel_ = stageData_->GetModels().empty() ? -1 : 0;
		}
	}

	ImGui::Separator();

	if (mode_ == EditMode::Collider) {
		if (ImGui::Button("Add AABB")) AddAABB();
		ImGui::SameLine();
		if (ImGui::Button("Duplicate") && selectedCollider_ >= 0) DuplicateAABB();
		ImGui::SameLine();
		if (ImGui::Button("Remove") && selectedCollider_ >= 0) RemoveAABB();
	} else {
		// 追加用入力
		ImGui::TextUnformatted("New Model");
		ImGui::InputText("NewName", newModelNameBuf_, sizeof(newModelNameBuf_));
		ImGui::InputText("NewModelName", newModelModelNameBuf_, sizeof(newModelModelNameBuf_));

		if (ImGui::Button("Add Model")) AddModel();
		ImGui::SameLine();
		if (ImGui::Button("Duplicate") && selectedModel_ >= 0) DuplicateModel();
		ImGui::SameLine();
		if (ImGui::Button("Remove") && selectedModel_ >= 0) RemoveModel();
	}
}

void CombatStageEditor::DrawModeSwitch() {
	ImGui::TextUnformatted("Mode");

	const bool isCollider = (mode_ == EditMode::Collider);
	const bool isModel = (mode_ == EditMode::Model);

	if (ImGui::RadioButton("Collider", isCollider)) {
		mode_ = EditMode::Collider;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Model", isModel)) {
		mode_ = EditMode::Model;
	}
}

void CombatStageEditor::DrawListCollider() {
	ImGui::TextUnformatted("AABB List");

	auto& list = stageData_->GetColliderMutable();

	for (int i = 0; i < static_cast<int>(list.size()); ++i) {
		auto& c = list[i];

		ImGui::PushID(i);

		// 有効
		ImGui::Checkbox("##Active", &c.isActive_);
		ImGui::SameLine();

		// 選択
		const bool isSel = (selectedCollider_ == i);
		const std::string label = c.name.empty() ? ("AABB_" + std::to_string(i)) : c.name;
		if (ImGui::Selectable(label.c_str(), isSel)) {
			selectedCollider_ = i;
		}

		ImGui::PopID();
	}
}

void CombatStageEditor::DrawInspectorCollider() {
	ImGui::TextUnformatted("Inspector");

	auto& list = stageData_->GetColliderMutable();
	if (selectedCollider_ < 0 || selectedCollider_ >= static_cast<int>(list.size())) {
		ImGui::TextUnformatted("No selection.");
		return;
	}

	auto& c = list[selectedCollider_];

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

	// min <= max
	c.min.x = std::min(c.min.x, c.max.x);
	c.min.y = std::min(c.min.y, c.max.y);
	c.min.z = std::min(c.min.z, c.max.z);

	c.max.x = std::max(c.min.x, c.max.x);
	c.max.y = std::max(c.min.y, c.max.y);
	c.max.z = std::max(c.min.z, c.max.z);

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
	selectedCollider_ = static_cast<int>(list.size()) - 1;
}

void CombatStageEditor::DuplicateAABB() {
	auto& list = stageData_->GetColliderMutable();
	if (selectedCollider_ < 0 || selectedCollider_ >= static_cast<int>(list.size())) return;

	CombatStage::AABB copy = list[selectedCollider_];
	// 名前
	copy.name += "_Copy";

	list.push_back(copy);
	selectedCollider_ = static_cast<int>(list.size()) - 1;
}

void CombatStageEditor::RemoveAABB() {
	auto& list = stageData_->GetColliderMutable();
	if (selectedCollider_ < 0 || selectedCollider_ >= static_cast<int>(list.size())) return;

	list.erase(list.begin() + selectedCollider_);

	if (list.empty()) {
		selectedCollider_ = -1;
	} else {
		selectedCollider_ = std::min(selectedCollider_, static_cast<int>(list.size()) - 1);
	}
}

void CombatStageEditor::DrawListModel() {
	ImGui::TextUnformatted("Model List");

	auto& list = stageData_->GetModelsMutable();

	for (int i = 0; i < static_cast<int>(list.size()); ++i) {
		auto& m = list[i];

		ImGui::PushID(i);

		// 有効
		ImGui::Checkbox("##Active", &m.isActive_);
		ImGui::SameLine();

		// 選択
		const bool isSel = (selectedModel_ == i);
		const std::string label = m.name.empty() ? ("Model_" + std::to_string(i)) : m.name;
		if (ImGui::Selectable(label.c_str(), isSel)) {
			selectedModel_ = i;
		}

		ImGui::PopID();
	}
}

void CombatStageEditor::DrawInspectorModel() {
	ImGui::TextUnformatted("Inspector");

	auto& list = stageData_->GetModelsMutable();
	if (selectedModel_ < 0 || selectedModel_ >= static_cast<int>(list.size())) {
		ImGui::TextUnformatted("No selection.");
		return;
	}

	auto& m = list[selectedModel_];

	// 管理名
	{
		char buf[256]{};
		std::snprintf(buf, sizeof(buf), "%s", m.name.c_str());
		if (ImGui::InputText("Name", buf, sizeof(buf))) {
			m.name = buf;
		}
	}

	// モデル名
	{
		char buf[256]{};
		std::snprintf(buf, sizeof(buf), "%s", m.modelName.c_str());
		if (ImGui::InputText("ModelName", buf, sizeof(buf))) {
			m.modelName = buf;
		}
	}

	// Transform
	ImGui::DragFloat3("Translate", &m.translate.x, 0.05f);
	ImGui::DragFloat3("Rotate", &m.rotate.x, 0.02f);
	ImGui::DragFloat3("Scale", &m.scale.x, 0.05f);

	// スケール下限
	m.scale.x = std::max(0.001f, m.scale.x);
	m.scale.y = std::max(0.001f, m.scale.y);
	m.scale.z = std::max(0.001f, m.scale.z);

	// 行列更新
	stageData_->UpdateModelWorldMatrix(m);
}

void CombatStageEditor::AddModel() {
	auto& list = stageData_->GetModelsMutable();

	CombatStageData::StageModelData m{};
	// 有効
	m.isActive_ = true;

	// 管理名
	m.name = newModelNameBuf_;
	if (m.name.empty()) {
		m.name = "Model_" + std::to_string(static_cast<int>(list.size()));
	}

	// モデル名
	m.modelName = newModelModelNameBuf_;
	if (m.modelName.empty()) {
		return;
	}

	// 念のためモデルをロード
	MAGISYSTEM::LoadCreateModel(m.modelName);

	// 初期TRS
	m.translate = { 0.0f,0.0f,0.0f };
	m.rotate = { 0.0f,0.0f,0.0f };
	m.scale = { 1.0f,1.0f,1.0f };

	// 行列更新
	stageData_->UpdateModelWorldMatrix(m);

	list.push_back(m);
	selectedModel_ = static_cast<int>(list.size()) - 1;

	// 次の追加用に管理名だけ自動更新
	std::snprintf(newModelNameBuf_, sizeof(newModelNameBuf_), "Model_%d", static_cast<int>(list.size()));
}


void CombatStageEditor::DuplicateModel() {
	auto& list = stageData_->GetModelsMutable();
	if (selectedModel_ < 0 || selectedModel_ >= static_cast<int>(list.size())) return;

	auto copy = list[selectedModel_];
	// 名前
	copy.name += "_Copy";

	stageData_->UpdateModelWorldMatrix(copy);

	list.push_back(copy);
	selectedModel_ = static_cast<int>(list.size()) - 1;
}

void CombatStageEditor::RemoveModel() {
	auto& list = stageData_->GetModelsMutable();
	if (selectedModel_ < 0 || selectedModel_ >= static_cast<int>(list.size())) return;

	list.erase(list.begin() + selectedModel_);

	if (list.empty()) {
		selectedModel_ = -1;
	} else {
		selectedModel_ = std::min(selectedModel_, static_cast<int>(list.size()) - 1);
	}
}
