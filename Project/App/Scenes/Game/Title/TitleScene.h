#pragma once

// フレームワーク
#include "MAGI.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene:public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~TitleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:

};