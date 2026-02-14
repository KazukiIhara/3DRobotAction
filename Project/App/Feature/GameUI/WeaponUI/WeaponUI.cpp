#include "WeaponUI.h"

#include "MAGI.h"

WeaponUI::WeaponUI() {
	// パラメータの追加
	MAGISYSTEM::AddParameterData({ "UI","WeaponUI","Pos" }, Magi::ParamType::Vec2);

	// テクスチャのロード
	MAGISYSTEM::LoadTexture("WeaponUI.png");

	// パラメータ取得
	screenPos_ = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","WeaponUI","Pos" });
	data_.position = screenPos_;

	// マテリアル設定
	mat_.textureName = "WeaponUI.png";

}

void WeaponUI::Update() {
	screenPos_ = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","WeaponUI","Pos" });
	data_.position = screenPos_;
}

void WeaponUI::Draw() {
	MAGISYSTEM::DrawSprite(data_, mat_);
}
