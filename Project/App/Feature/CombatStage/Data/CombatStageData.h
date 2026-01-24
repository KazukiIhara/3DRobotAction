#pragma once

// C++
#include <string>
#include <vector>

#include "Feature/CombatStage/CombatStage.h"

/// <summary>
/// ステージのデータ
/// </summary>
class CombatStageData {
public:
	struct DebugFlag {
		bool showImGui = false;
		bool isDrawCollider = false;
	};
public:
	CombatStageData();
	~CombatStageData() = default;

	void Draw();

	// コライダー取得
	const std::vector<CombatStage::AABB>& GetCollider() const;
	// 編集用
	std::vector<CombatStage::AABB>& GetColliderMutable();

	// 追加
	void AddCollider(const CombatStage::AABB& aabb);
	// クリア
	void ClearCollider();

	// Json入出力
	bool SaveColliderJson(const std::string& path) const;
	bool LoadColliderJson(const std::string& path);

	const DebugFlag& GetDebugFlag()const;

	void SwitchShowImGui();
private:
	void DrawImGui();

	void SwitchIsDrawCollider();
private:
	// コライダーのリスト
	std::vector<CombatStage::AABB> colliders_;

	DebugFlag debugFlag_{};
};