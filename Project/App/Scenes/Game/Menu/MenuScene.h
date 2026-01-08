#pragma once

#include <memory>
#include <array>

#include "MAGI.h"

/// <summary>
/// メニューシーン ひとまず操作説明が出るシーンとする
/// </summary>
class MenuScene:public Magi::BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~MenuScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// 連打防止タイマー
	float timer_ = 2.0f;

	// 黒背景スプライト
	SpriteData black_;
	SpriteMaterialData blackMat_;

	// 操作説明スプライト
	SpriteData h2pSheet_;
	SpriteMaterialData h2pSheetMat_;

};