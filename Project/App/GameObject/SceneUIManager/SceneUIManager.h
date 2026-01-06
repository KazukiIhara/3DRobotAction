#pragma once

// C++
#include <unordered_map>
#include <string>
#include <memory>

// 2DUIクラス
#include "GameObject/GameUI2D/GameUI2D.h"

/// <summary>
/// ゲームUIマネージャ
/// </summary>
class SceneUIManager {
public:
	SceneUIManager(const std::string& sceneName);
	~SceneUIManager() = default;

	void Add(std::unique_ptr<GameUI2D> ui);

private:
	void Clear();

private:
	// シーン名
	std::string sceneName_;
	// UIのデータパス
	std::string	uiDataPath_;

	// 更新用コンテナ
	std::vector <std::unique_ptr<GameUI2D>> sceneUIList_;
	// コンテナ
	std::unordered_map<std::string, GameUI2D*> sceneUIMap_;
};