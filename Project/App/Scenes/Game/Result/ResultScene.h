#pragma once

// フレームワーク
#include "MAGI.h"

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene :public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~ResultScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	SpriteData data_{};
	SpriteMaterialData mat_{};

	SpriteData uidData_{};
	SpriteMaterialData uiMat_{};

};