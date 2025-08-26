#include "TitleScene.h"

void TitleScene::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// 3Dカメラ作成
	std::shared_ptr<Camera3D> sceneCamera3D = std::make_shared<Camera3D>("SceneCamera3D");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera3D)).lock()->ApplyCurrent();


	// 
	// アセットのロード
	// 


	// テクスチャ

	MAGISYSTEM::LoadTexture("TitleBG.png");
	MAGISYSTEM::LoadTexture("Start.png");
	MAGISYSTEM::LoadTexture("Exit.png");

	//===================================
	// テクスチャのロード
	//===================================

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

	//===================================
	// モデルのロード
	//===================================

	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::CreateModelDrawer("teapot", MAGISYSTEM::FindModel("teapot"));

	MAGISYSTEM::LoadModel("StageObj0");
	MAGISYSTEM::CreateModelDrawer("StageObj0", MAGISYSTEM::FindModel("StageObj0"));

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));

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

	MAGISYSTEM::LoadModel("AssultRifle");
	MAGISYSTEM::CreateModelDrawer("AssultRifle", MAGISYSTEM::FindModel("AssultRifle"));

	MAGISYSTEM::LoadModel("Bullet");
	MAGISYSTEM::CreateModelDrawer("Bullet", MAGISYSTEM::FindModel("Bullet"));



	startData_.position = { WindowApp::kClientWidth * 0.5f, 900.0f };
	exitData_.position = { WindowApp::kClientWidth * 0.5f, 912.0f };

	bgMatData_.textureName = "TitleBG.png";

	startMatData_.textureName = "Start.png";
	startMatData_.anchorPoint = { 0.5f,0.5f };

	exitMatData_.textureName = "Exit.png";
	exitMatData_.anchorPoint = { 0.5f,0.5f };

}

void TitleScene::Update() {

	if (MAGISYSTEM::IsPadConnected(0)) {
		if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
			this->ChangeScene("Play");
		}
	}

}

void TitleScene::Draw() {
	MAGISYSTEM::DrawSprite(bgData_, bgMatData_);

	MAGISYSTEM::DrawSprite(startData_, startMatData_);

}

void TitleScene::Finalize() {

}
