#include "MenuScene.h"

using namespace Magi;

void MenuScene::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// 3Dカメラ作成
	std::unique_ptr<Camera3D> sceneCamera3D = std::make_unique<Camera3D>("SceneCamera3D");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera3D))->ApplyCurrent();

	// 黒背景
	blackMat_.textureName = "TitleBG.png";

	// 操作説明シート
	h2pSheet_.position = { WindowApp::kClientWidth * 0.5f,WindowApp::kClientHeight * 0.5f };

	h2pSheetMat_.anchorPoint = { 0.5f,0.5f };
	h2pSheetMat_.textureName = "HowToPlay.png";

	// 最低でも2秒間シーンにとどまるためのタイマー
	timer_ = 2.0f;
}

void MenuScene::Update() {
	// 連打防止タイマー更新
	timer_ -= 1.0f * MAGISYSTEM::GetDeltaTime();
	timer_ = std::max(timer_, 0.0f);

	if (!timer_) {
		if (MAGISYSTEM::IsPadConnected(0)) {
			if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
				this->ChangeScene("Play");
			}
		} else {
			if (MAGISYSTEM::TriggerKey(DIK_SPACE)) {
				this->ChangeScene("Play");
			}
		}
	}

}

void MenuScene::Draw() {
	// 黒背景
	MAGISYSTEM::DrawSprite(black_, blackMat_);
	// 操作説明シート
	MAGISYSTEM::DrawSprite(h2pSheet_, h2pSheetMat_);
}

void MenuScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}