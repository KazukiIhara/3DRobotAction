#pragma once

// C++
#include <memory>
#include <array>
#include <string>

// フレームワーク
#include "MAGI.h"

// エフェクトマネージャ
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

/// <summary>
/// エフェクト作成シーン
/// </summary>
class EffectEditScene:public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~EffectEditScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:

	// ゲームエフェクトマネージャ
	std::unique_ptr<GameEffectManager> gameEffectManager_;
};
