#include "ResultScene.h"

using namespace Magi;

void ResultScene::Initialize() {
	// テクスチャをロード
	MAGISYSTEM::LoadTexture("white.png");

	// サイズを設定
	data_.size = { WindowApp::kClientWidth,WindowApp::kClientHeight };
	// テクスチャを設定
	mat_.textureName = "white.png";
	mat_.color = Color::Black;

}

void ResultScene::Update() {
	// 入力を受け取ってタイトルシーンへ
	if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
		ChangeScene("Title");
	}
}

void ResultScene::Draw() {
	MAGISYSTEM::DrawSprite(data_, mat_);
}

void ResultScene::Finalize() {

}
