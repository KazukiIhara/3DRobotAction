#include "TitleScene.h"

using namespace Magi;

void TitleScene::Initialize() {
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


	// 
	// アセットのロード
	// 



	//===================================
	// テクスチャのロード
	//===================================

	// UIテクスチャ
	MAGISYSTEM::LoadTexture("TitleBG.png");
	MAGISYSTEM::LoadTexture("Start.png");
	MAGISYSTEM::LoadTexture("Exit.png");

	// ロックオン用テクスチャ
	MAGISYSTEM::LoadTexture("LockonUIGray.png");
	MAGISYSTEM::LoadTexture("LockonUIRed.png");

	// APUI用テクスチャ
	MAGISYSTEM::LoadTexture("ApGauge.png");
	MAGISYSTEM::LoadTexture("ApBar.png");

	MAGISYSTEM::LoadTexture("BossApBar.png");
	MAGISYSTEM::LoadTexture("BossApGauge.png");

	// ENUI用テクスチャ
	MAGISYSTEM::LoadTexture("EnBar.png");
	MAGISYSTEM::LoadTexture("EnGauge.png");

	MAGISYSTEM::LoadTexture("EnergyUI.png");
	MAGISYSTEM::LoadTexture("EnergyUI_Bar.png");
	MAGISYSTEM::LoadTexture("EnergyUI_Gauge.png");

	// 武器UI用テクスチャ

	// 右手表示
	MAGISYSTEM::LoadTexture("Right.png");
	// 左手表示
	MAGISYSTEM::LoadTexture("Left.png");


	// 円形テクスチャ
	MAGISYSTEM::LoadTexture("Circle2.png");

	// ホワイトテクスチャ
	MAGISYSTEM::LoadTexture("white.png");

	// 靄用
	MAGISYSTEM::LoadTexture("smoke.png");
	// リング用
	MAGISYSTEM::LoadTexture("gradation.png");
	// 粒子用
	MAGISYSTEM::LoadTexture("star.png");
	// マズルフラッシュ
	MAGISYSTEM::LoadTexture("muzzleFlash.png");

	// プレイ開始テクスチャ
	MAGISYSTEM::LoadTexture("StartUI_A.png");
	MAGISYSTEM::LoadTexture("StartUI_B.png");
	MAGISYSTEM::LoadTexture("StartUI_E.png");
	MAGISYSTEM::LoadTexture("StartUI_L.png");
	MAGISYSTEM::LoadTexture("StartUI_R.png");
	MAGISYSTEM::LoadTexture("StartUI_S.png");
	MAGISYSTEM::LoadTexture("StartUI_T.png");

	// 終了時テクスチャ
	MAGISYSTEM::LoadTexture("YouWin.png");
	MAGISYSTEM::LoadTexture("YouLose.png");

	// 操作説明UI
	MAGISYSTEM::LoadTexture("HowToPlay.png");

	//===================================
	// モデルのロード
	//===================================

	// 
	// デバッグ用
	// 

	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::CreateModelDrawer("teapot", MAGISYSTEM::FindModel("teapot"));

	// 
	// 地形
	// 

	MAGISYSTEM::LoadModel("StageObj0");
	MAGISYSTEM::CreateModelDrawer("StageObj0", MAGISYSTEM::FindModel("StageObj0"));

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));

	// 
	// 機体
	// 

	MAGISYSTEM::LoadModel("MechHead");
	MAGISYSTEM::CreateModelDrawer("MechHead", MAGISYSTEM::FindModel("MechHead"));

	MAGISYSTEM::LoadModel("MechBody");
	MAGISYSTEM::CreateModelDrawer("MechBody", MAGISYSTEM::FindModel("MechBody"));

	MAGISYSTEM::LoadModel("MechRightArm");
	MAGISYSTEM::CreateModelDrawer("MechRightArm", MAGISYSTEM::FindModel("MechRightArm"));

	MAGISYSTEM::LoadModel("MechLeftArm");
	MAGISYSTEM::CreateModelDrawer("MechLeftArm", MAGISYSTEM::FindModel("MechLeftArm"));

	MAGISYSTEM::LoadModel("MechLeg");
	MAGISYSTEM::CreateModelDrawer("MechLeg", MAGISYSTEM::FindModel("MechLeg"));

	//
	// 武器
	//

	MAGISYSTEM::LoadModel("AssultRifle");
	MAGISYSTEM::CreateModelDrawer("AssultRifle", MAGISYSTEM::FindModel("AssultRifle"));

	MAGISYSTEM::LoadModel("RocketLauncher");
	MAGISYSTEM::CreateModelDrawer("RocketLauncher", MAGISYSTEM::FindModel("RocketLauncher"));

	MAGISYSTEM::LoadModel("DualMissileLauncher");
	MAGISYSTEM::CreateModelDrawer("DualMissileLauncher", MAGISYSTEM::FindModel("DualMissileLauncher"));

	// 
	// 弾など
	// 
	MAGISYSTEM::LoadModel("Bullet");
	MAGISYSTEM::CreateModelDrawer("Bullet", MAGISYSTEM::FindModel("Bullet"));

	MAGISYSTEM::LoadModel("Missile");
	MAGISYSTEM::CreateModelDrawer("Missile", MAGISYSTEM::FindModel("Missile"));

	// 
	// エフェクト
	// 
	MAGISYSTEM::LoadModel("Spark");
	MAGISYSTEM::CreateModelDrawer("Spark", MAGISYSTEM::FindModel("Spark"));


	// 
	// パラメータ作成
	// 
	MAGISYSTEM::AddParameterGroup("TitleScene");

	MAGISYSTEM::AddParameterTag({ "TitleScene","UIPosition" });

	MAGISYSTEM::AddParameterData({ "TitleScene","UIPosition","StartUIPos" }, ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "TitleScene","UIPosition","ExitUIPos" }, ParamType::Vec2);

	// シーンUI設定
	startData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "TitleScene", "UIPosition", "StartUIPos" });
	exitData_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "TitleScene", "UIPosition", "ExitUIPos" });

	bgMatData_.textureName = "TitleBG.png";

	startMatData_.textureName = "Start.png";
	startMatData_.anchorPoint = { 0.5f,0.5f };

	exitMatData_.textureName = "Exit.png";
	exitMatData_.anchorPoint = { 0.5f,0.5f };

}

void TitleScene::Update() {

	if (MAGISYSTEM::IsPadConnected(0)) {
		if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
			this->ChangeScene("Menu");
		}
	} else {
		if (MAGISYSTEM::TriggerKey(DIK_SPACE)) {
			this->ChangeScene("Menu");
		}
	}

}

void TitleScene::Draw() {
	MAGISYSTEM::DrawSprite(bgData_, bgMatData_);
	MAGISYSTEM::DrawSprite(startData_, startMatData_);
}

void TitleScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}
