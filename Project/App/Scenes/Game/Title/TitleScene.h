#pragma once

#include <memory>
#include <array>

#include "MAGI.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~TitleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	SpriteData bgData_;
	SpriteMaterialData bgMatData_;
	SpriteData startData_;
	SpriteMaterialData startMatData_;
	SpriteData exitData_;
	SpriteMaterialData exitMatData_;
};

