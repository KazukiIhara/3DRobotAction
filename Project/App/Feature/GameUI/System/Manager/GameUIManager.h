#pragma once

// C++
#include <memory>
#include <vector>

#include "Feature/GameUI/System/Base/BaseGameUI.h"

/// <summary>
/// ゲームUIマネージャ
/// </summary>
class GameUIManager {
public:
	GameUIManager();
	~GameUIManager() = default;

	BaseGameUI* Add(std::unique_ptr<BaseGameUI> ui);

	void Update();
	void Draw();

private:
	void DeleteGarbage();

private:
	// UIコンテナ
	std::vector<std::unique_ptr<BaseGameUI>> uis_;
};
