#pragma once

// C++
#include <string>

class CombatStageData;

/// <summary>
/// ステージ編集（AABB + モデル配置）
/// </summary>
class CombatStageEditor {
public:
	explicit CombatStageEditor(CombatStageData* stageData);
	~CombatStageEditor() = default;

	void DrawImGui();

	void SetPath(const std::string& path);

private:
	enum class EditMode {
		Collider,
		Model,
	};

	void DrawToolbar();
	void DrawModeSwitch();

	// Collider
	void DrawListCollider();
	void DrawInspectorCollider();
	void AddAABB();
	void DuplicateAABB();
	void RemoveAABB();

	// Model
	void DrawListModel();
	void DrawInspectorModel();
	void AddModel();
	void DuplicateModel();
	void RemoveModel();

private:
	CombatStageData* stageData_ = nullptr;

	EditMode mode_ = EditMode::Collider;

	int selectedCollider_ = -1;
	int selectedModel_ = -1;

	std::string path_{ "Assets/StageData/CombatStage.json" };
	char pathBuf_[512]{};

	// 追加用入力
	char newModelNameBuf_[256]{};
	char newModelModelNameBuf_[256]{};
};
