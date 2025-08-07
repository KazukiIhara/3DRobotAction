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

inline void TitleScene::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// 3Dカメラ作成
	std::shared_ptr<Camera3D> sceneCamera3D = std::make_shared<Camera3D>("SceneCamera3D");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera3D)).lock()->ApplyCurrent();


	// 
	// アセットのロード
	// 


	// テクスチャ

	MAGISYSTEM::LoadTexture("TitleBG.png");
	MAGISYSTEM::LoadTexture("Start.png");
	MAGISYSTEM::LoadTexture("Exit.png");

	startData_.position = { WindowApp::kClientWidth * 0.5f, 800.0f };
	exitData_.position = { WindowApp::kClientWidth * 0.5f, 912.0f };

	bgMatData_.textureName = "TitleBG.png";

	startMatData_.textureName = "Start.png";
	startMatData_.anchorPoint = { 0.5f,0.5f };

	exitMatData_.textureName = "Exit.png";
	exitMatData_.anchorPoint = { 0.5f,0.5f };

}

inline void TitleScene::Update() {

	if (MAGISYSTEM::IsPadConnected(0)) {
		if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
			this->ChangeScene("Play");
		}
	}

}

inline void TitleScene::Draw() {
	MAGISYSTEM::DrawSprite(bgData_, bgMatData_);

	MAGISYSTEM::DrawSprite(startData_, startMatData_);
	MAGISYSTEM::DrawSprite(exitData_, exitMatData_);

}

inline void TitleScene::Finalize() {

}
