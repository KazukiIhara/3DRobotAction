#pragma once

// C++
#include <string>

class CombatStageData;

class CombatStageEditor {
public:
	explicit CombatStageEditor(CombatStageData* stageData);
	~CombatStageEditor() = default;

	// ImGui描画
	void DrawImGui();

	// パス設定
	void SetPath(const std::string& path);

private:
	void DrawToolbar();
	void DrawList();
	void DrawInspector();

	void AddAABB();
	void Duplicate();
	void Remove();

private:
	CombatStageData* stageData_ = nullptr;

	int selected_ = -1;

	std::string path_{ "Assets/StageData/CombatStage.json" };
	char pathBuf_[512]{};
};
