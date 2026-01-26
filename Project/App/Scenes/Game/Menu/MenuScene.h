#pragma once

// フレームワーク
#include "MAGI.h"

/// <summary>
/// メニューシーン
/// </summary>
class MenuScene:public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~MenuScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:

};