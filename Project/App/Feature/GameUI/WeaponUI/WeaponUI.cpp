#include "WeaponUI.h"

#include "MAGI.h"

WeaponUI::WeaponUI() {
	// パラメータの追加
	MAGISYSTEM::AddParameterData({ "UI","CannonRifle","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","MachineGun","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","LT","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","RT","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","LB","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","RB","Pos" }, Magi::ParamType::Vec2);

	// テクスチャのロード
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
	cannonRifle_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","CannonRifle","Pos" });
	machineGun_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","MachineGun","Pos" });
	lt_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LT","Pos" });
	rt_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RT","Pos" });
	lb_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LB","Pos" });
	rb_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RB","Pos" });

	// マテリアル設定
	cannonRifleMat_.textureName = "CannonRifleUI.png";
	machineGunMat_.textureName = "MachineGunUI.png";

	ltMat_.textureName = "LT.png";
	rtMat_.textureName = "RT.png";
	lbMat_.textureName = "LB.png";
	rbMat_.textureName = "RB.png";

}

void WeaponUI::Update() {
	cannonRifle_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","CannonRifle","Pos" });
	machineGun_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","MachineGun","Pos" });
	lt_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LT","Pos" });
	rt_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RT","Pos" });
	lb_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LB","Pos" });
	rb_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RB","Pos" });


	// ボタンテクスチャ切り替え

}

void WeaponUI::Draw() {
	MAGISYSTEM::DrawSprite(cannonRifle_, cannonRifleMat_);
	MAGISYSTEM::DrawSprite(machineGun_, machineGunMat_);

	MAGISYSTEM::DrawSprite(lt_, ltMat_);
	MAGISYSTEM::DrawSprite(rt_, rtMat_);
	MAGISYSTEM::DrawSprite(lb_, lbMat_);
	MAGISYSTEM::DrawSprite(rb_, rbMat_);
}
