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
	// DirectionalLight
	DirectionalLight directionalLight_{};

	SpriteData data_{};
	SpriteMaterialData mat_{};

	SpriteData logo_{};
	SpriteMaterialData logoMat_{};

	SpriteData uiData_;
	SpriteMaterialData uiMat_;

	SpriteData uiEffectData_;
	SpriteMaterialData uiEffectMat_;

	float t_ = 0.0f;
};