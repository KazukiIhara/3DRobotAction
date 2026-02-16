#include "WeaponUI.h"

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/Pilot/Mech/Weapon/MachineGun/PilotMechWeaponMachineGun.h"

using namespace MAGIUtility;

WeaponUI::WeaponUI(PilotMech* pilotMech) {

	pilotMech_ = pilotMech;

	// パラメータの追加
	MAGISYSTEM::AddParameterData({ "UI","CannonRifle","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","MachineGun","Pos" }, Magi::ParamType::Vec2);

	MAGISYSTEM::AddParameterData({ "UI","Jump","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","Dodge","Pos" }, Magi::ParamType::Vec2);

	MAGISYSTEM::AddParameterData({ "UI","LT","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","RT","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","LB","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","RB","Pos" }, Magi::ParamType::Vec2);

	MAGISYSTEM::AddParameterData({ "UI","Ammo","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","Ammo","Size" }, Magi::ParamType::Float);

	// テクスチャのロード
	MAGISYSTEM::LoadTexture("CannonRifleUI.png");
	MAGISYSTEM::LoadTexture("MachineGunUI.png");

	MAGISYSTEM::LoadTexture("Dodge.png");
	MAGISYSTEM::LoadTexture("Jump.png");

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

	// パイロットのスクリーン座標を取得
	const Vector3 pilotWPos = pilotMech_->GetCenterPos();
	const Vector2 pilotScreenPos = TransformWorldToScreen(pilotWPos);

	lb_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LB","Pos" });
	rb_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RB","Pos" });

	jump_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Jump","Pos" });
	dodge_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Dodge","Pos" });

	ammoPos_ = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Ammo","Pos" });
	ammoSize_ = MAGISYSTEM::GetParameterValue<float>({ "UI","Ammo","Size" });

	// マテリアル設定
	cannonRifleMat_.textureName = "CannonRifleUI.png";
	machineGunMat_.textureName = "MachineGunUI.png";

	ltMat_.textureName = "LT.png";
	rtMat_.textureName = "RT.png";

	lbMat_.textureName = "LB.png";
	lbMat_.anchorPoint = { 0.5f,0.5f };
	rbMat_.textureName = "RB.png";
	rbMat_.anchorPoint = { 0.5f,0.5f };

	jumpMat_.textureName = "Jump.png";
	jumpMat_.anchorPoint = { 0.5f,0.5f };
	dodgeMat_.textureName = "Dodge.png";
	dodgeMat_.anchorPoint = { 0.5f,0.5f };

}

void WeaponUI::Update() {
	// UIの座標を設定
	cannonRifle_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","CannonRifle","Pos" });
	machineGun_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","MachineGun","Pos" });
	lt_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LT","Pos" });
	rt_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RT","Pos" });

	ammoPos_ = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Ammo","Pos" });
	ammoSize_ = MAGISYSTEM::GetParameterValue<float>({ "UI","Ammo","Size" });

	// パイロットのスクリーン座標を取得
	const Vector3 pilotWPos = pilotMech_->GetCenterPos();
	const Vector2 pilotScreenPos = TransformWorldToScreen(pilotWPos);

	lb_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LB","Pos" });
	rb_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","RB","Pos" });

	jump_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Jump","Pos" });
	dodge_.position = pilotScreenPos + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","Dodge","Pos" });

	// 残弾に応じて武器ボタンの色を変える
	const int32_t mgAmmo = dynamic_cast<PilotMechWeaponMachineGun*>(pilotMech_->GetWeapon("MachineGun"))->GetAmmo();
	// 球数が0なら灰色にする
	if (mgAmmo <= 0) {
		rtMat_.textureName = "RT.png";
	} else {
		rtMat_.textureName = "RTEnable.png";
	}

	// ボタンテクスチャ切り替え
	PilotMech::State cs = pilotMech_->GetCurrentState();
	// ステートごとに切り替え
	switch (cs) {
	case PilotMech::State::Idle:
		ltMat_.textureName = "LT.png";
		lbMat_.textureName = "LBEnable.png";
		rbMat_.textureName = "RBEnable.png";
		break;
	case PilotMech::State::Move:
		ltMat_.textureName = "LT.png";
		lbMat_.textureName = "LBEnable.png";
		rbMat_.textureName = "RBEnable.png";
		break;
	case PilotMech::State::Dodge:
		ltMat_.textureName = "LT.png";
		lbMat_.textureName = "LBEnable.png";
		rbMat_.textureName = "RB.png";
		break;
	case PilotMech::State::JustDodge:
		ltMat_.textureName = "LTEnable.png";
		lbMat_.textureName = "LBEnable.png";
		rbMat_.textureName = "RB.png";
		break;
	case PilotMech::State::JustDodgeAttack:
		ltMat_.textureName = "LT.png";
		lbMat_.textureName = "LB.png";
		rbMat_.textureName = "RB.png";
		break;
	case PilotMech::State::HitReact:
		ltMat_.textureName = "LT.png";
		lbMat_.textureName = "LB.png";
		rbMat_.textureName = "RB.png";
		break;
	case PilotMech::State::KnockBack:
		ltMat_.textureName = "LT.png";
		lbMat_.textureName = "LB.png";
		rbMat_.textureName = "RB.png";
		break;
	default:
		break;
	}
}

void WeaponUI::Draw() {

	MAGISYSTEM::DrawSprite(jump_, jumpMat_);
	MAGISYSTEM::DrawSprite(dodge_, dodgeMat_);
	MAGISYSTEM::DrawSprite(lb_, lbMat_);
	MAGISYSTEM::DrawSprite(rb_, rbMat_);

	MAGISYSTEM::DrawSprite(cannonRifle_, cannonRifleMat_);
	MAGISYSTEM::DrawSprite(machineGun_, machineGunMat_);

	MAGISYSTEM::DrawSprite(lt_, ltMat_);
	MAGISYSTEM::DrawSprite(rt_, rtMat_);

	const int32_t mgAmmo = dynamic_cast<PilotMechWeaponMachineGun*>(pilotMech_->GetWeapon("MachineGun"))->GetAmmo();
	MAGISYSTEM::DrawFont(std::to_string(mgAmmo) + "/30", ammoPos_, Color::White, ammoSize_);
}
