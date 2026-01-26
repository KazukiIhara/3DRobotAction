#pragma once

// フレームワーク
#include "MAGI.h"

/// <summary>
/// 戦闘シーン
/// </summary>
class CombatScene :public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~CombatScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:

};