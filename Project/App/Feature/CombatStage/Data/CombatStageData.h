#pragma once

// C++
#include <string>
#include <vector>

#include "Feature/CombatStage/CombatStage.h"
#include "Math/Types/Vector3.h"
#include "Math/Types/Matrix4x4.h"

class CombatStageEditor;

/// <summary>
/// ステージのデータ（コライダー + モデル配置）
/// </summary>
class CombatStageData {
public:
	CombatStageData();
	~CombatStageData() = default;

	void Draw();

	const std::vector<CombatStage::AABB>& GetCollider() const;
	std::vector<CombatStage::AABB>& GetColliderMutable();

	struct StageModelData {
		// 管理名
		std::string name;
		// 描画モデル名
		std::string modelName;
		// 有効
		bool isActive_ = true;

		// TRS
		Vector3 translate{ 0.0f,0.0f,0.0f };
		Vector3 rotate{ 0.0f,0.0f,0.0f };
		Vector3 scale{ 1.0f,1.0f,1.0f };

		// 描画用ワールド行列
		Matrix4x4 worldMatrix{};
	};

	const std::vector<StageModelData>& GetModels() const;
	std::vector<StageModelData>& GetModelsMutable();

	void AddCollider(const CombatStage::AABB& aabb);
	void ClearCollider();

	void AddModel(const StageModelData& model);
	void ClearModels();

	bool SaveJson(const std::string& path) const;
	bool LoadJson(const std::string& path);

	struct DebugFlag {
		bool showImGui = false;
		bool isDrawCollider = true;
	};

	const DebugFlag& GetDebugFlag() const;

	void SwitchShowImGui();

private:
	void DrawImGui();
	void SwitchIsDrawCollider();

	void UpdateModelWorldMatrix(StageModelData& model);

private:
	std::vector<CombatStage::AABB> colliders_;
	std::vector<StageModelData> models_;

	DebugFlag debugFlag_{};

	friend class CombatStageEditor;
};
