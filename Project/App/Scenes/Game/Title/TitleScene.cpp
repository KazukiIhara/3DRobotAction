#include "TitleScene.h"

#include "SimpleAnimation/SimpleAnimation.h"

using namespace Magi;

void TitleScene::Initialize() {
	// パラメータ追加
	MAGISYSTEM::AddParameterData({ "UI","Title","PressAnyButton" }, Magi::ParamType::Vec2);

	MAGISYSTEM::AddParameterData({ "UI","Title","SizeEffectStart" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","Title","SizeEffectEnd" }, Magi::ParamType::Vec2);

	MAGISYSTEM::AddParameterData({ "UI","Title","AlphaEffectStart" }, Magi::ParamType::Float);
	MAGISYSTEM::AddParameterData({ "UI","Title","AlphaEffectEnd" }, Magi::ParamType::Float);

	// テクスチャをロード
	MAGISYSTEM::LoadTexture("White.png");
	MAGISYSTEM::LoadTexture("Title.png");
	MAGISYSTEM::LoadTexture("PressAButton.png");

	// サイズを設定
	data_.size = { WindowApp::kClientWidth,WindowApp::kClientHeight };
	// テクスチャを設定
	mat_.textureName = "white.png";
	mat_.color = Color::Black;

	logo_.position = { WindowApp::kClientWidth * 0.5f,WindowApp::kClientHeight * 0.5f };
	logoMat_.textureName = "Title.png";
	logoMat_.anchorPoint = { 0.5f,0.5f };

	// 座標設定
	uiData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","PressAnyButton" });
	uiMat_.textureName = "PressAButton.png";
	uiMat_.anchorPoint = { 0.5f,0.5f };

	uiEffectData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","PressAnyButton" });
	uiEffectMat_.textureName = "PressAButton.png";
	uiEffectMat_.anchorPoint = { 0.5f,0.5f };

}

void TitleScene::Update() {
	// 入力を受け取って戦闘シーンへ
	if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
		ChangeScene("Combat");
	}

	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	t_ += dt * 0.7f;
	if (t_ >= 1.0f) {
		t_ = 0.0f;
	}

	// UIアニメーション
	const Vector2 sSize = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","SizeEffectStart" });
	const Vector2 eSize = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","SizeEffectEnd" });
	uiEffectData_.size = SimpleAnimation<Vector2>(sSize, eSize).GetValue(t_);

	const float sAlpha = MAGISYSTEM::GetParameterValue<float>({ "UI","Title","AlphaEffectStart" });
	const float eAlpha = MAGISYSTEM::GetParameterValue<float>({ "UI","Title","AlphaEffectEnd" });
	uiEffectMat_.color.w = SimpleAnimation<float>(sAlpha, eAlpha).GetValue(t_);


	uiData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","PressAnyButton" });
	uiEffectData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Title","PressAnyButton" });

}

void TitleScene::Draw() {
	MAGISYSTEM::DrawSprite(data_, mat_);
	MAGISYSTEM::DrawSprite(uiData_, uiMat_);
	MAGISYSTEM::DrawSprite(uiEffectData_, uiEffectMat_);
	MAGISYSTEM::DrawSprite(logo_, logoMat_);
}

void TitleScene::Finalize() {

}
