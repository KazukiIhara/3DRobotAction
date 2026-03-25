#include "ResultScene.h"

using namespace Magi;

void ResultScene::Initialize() {

	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "UI","Title","PressAtoTitle","Pos" }, Magi::ParamType::Vec2);

	// テクスチャをロード
	MAGISYSTEM::LoadTexture("White.png");
	MAGISYSTEM::LoadTexture("PressAtoTitle.png");

	// サイズを設定
	data_.size = { WindowApp::kClientWidth,WindowApp::kClientHeight };
	// テクスチャを設定
	mat_.textureName = "white.png";
	mat_.color = Color::Black;

	// UIの設定
	uidData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","PressAtoTitle","Pos" });
	uiMat_.textureName = "PressAtoTitle.png";
	uiMat_.anchorPoint = { 0.5f,0.5f };

}

void ResultScene::Update() {
	// 入力を受け取ってタイトルシーンへ
	if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
		ChangeScene("Title");
	}


	// 座標設定
	uidData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","PressAtoTitle","Pos" });
}

void ResultScene::Draw() {
	MAGISYSTEM::DrawSprite(data_, mat_);
	MAGISYSTEM::DrawSprite(uidData_, uiMat_);
}

void ResultScene::Finalize() {

}
