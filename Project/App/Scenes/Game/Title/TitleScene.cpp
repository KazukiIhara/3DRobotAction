#include "TitleScene.h"

using namespace Magi;

void TitleScene::Initialize() {
	// テクスチャをロード
	MAGISYSTEM::LoadTexture("white.png");
	// サイズを設定
	data_.size = { WindowApp::kClientWidth,WindowApp::kClientHeight };
	// テクスチャを設定
	mat_.textureName = "white.png";
}

void TitleScene::Update() {

	// 入力を受け取って戦闘シーンへ
	if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
		ChangeScene("Combat");
	}

}

void TitleScene::Draw() {

}

void TitleScene::Finalize() {

}
