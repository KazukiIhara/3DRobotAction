#include "WeaponUI.h"

#include "MAGI.h"

WeaponUI::WeaponUI() {
	// パラメータの追加
	MAGISYSTEM::AddParameterData({ "UI","WeaponUI","Pos" }, Magi::ParamType::Vec2);

	// テクスチャのロード
	MAGISYSTEM::LoadTexture("WeaponUI.png");
	MAGISYSTEM::LoadTexture("CannonRifleUI.png");
	MAGISYSTEM::LoadTexture("MachineGunUI.png");

	MAGISYSTEM::LoadTexture("LT.png");
	MAGISYSTEM::LoadTexture("RT.png");
	MAGISYSTEM::LoadTexture("LB.png");
	MAGISYSTEM::LoadTexture("RB.png");

	MAGISYSTEM::LoadTexture("LBEnable.png");
	MAGISYSTEM::LoadTexture("RBEnable.png");
	MAGISYSTEM::LoadTexture("LTEnable.png");
	MAGISYSTEM::LoadTexture("RTEnable.png");


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
