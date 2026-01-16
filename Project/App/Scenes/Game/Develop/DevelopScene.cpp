#include "DevelopScene.h"

using namespace Magi;

void DevelopScene::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");
	// ライトを設定
	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));


	// 
	// リソースロード
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

	MAGISYSTEM::LoadModel("AssultRifle");
	MAGISYSTEM::CreateModelDrawer("AssultRifle", MAGISYSTEM::FindModel("AssultRifle"));

	MAGISYSTEM::LoadModel("RocketLauncher");
	MAGISYSTEM::CreateModelDrawer("RocketLauncher", MAGISYSTEM::FindModel("RocketLauncher"));

	MAGISYSTEM::LoadModel("DualMissileLauncher");
	MAGISYSTEM::CreateModelDrawer("DualMissileLauncher", MAGISYSTEM::FindModel("DualMissileLauncher"));

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));

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
	// 靄用
	MAGISYSTEM::LoadTexture("smoke.png");

	// マズルフラッシュ
	MAGISYSTEM::LoadTexture("muzzleFlash.png");


	//===========================
	// マネージャの初期化
	//===========================

	// ゲームエフェクトマネージャ
	gameEffectManager_ = std::make_unique<GameEffectManager>();

	// 攻撃コリジョンマネージャ
	attackCollisionManager_ = std::make_unique<AttackCollisionManager>();

	// 弾マネージャ
	attackObjectManger_ = std::make_unique<AttackObjectManager>(attackCollisionManager_.get());

	// コリジョンマネージャ
	damageCollisionSystem_ = std::make_unique<DamageCollisionSystem>();

	// 攻撃オブジェクトマネージャ
	damageObjectManager_ = std::make_unique<DamageObjectManager>(damageCollisionSystem_.get());


	// プレイヤー作成
	player_ = std::make_unique<Player>(attackObjectManger_.get(), gameEffectManager_.get());
	player_->SetIsOperation(true);

	// ボス作成
	boss_ = std::make_unique<Boss>(damageObjectManager_.get(), gameEffectManager_.get(), player_->GetMechCore());

	// 床追加
	MAGISYSTEM::LoadSceneDataFromJson("SceneData");
	MAGISYSTEM::ImportSceneData("SceneData", true);

}

void DevelopScene::Update() {
#if defined(DEBUG) || defined(DEVELOP)
	ImGui::Begin("DevelopUI");

	ImGui::SeparatorText("Scene");
	if (ImGui::Button("ResetScene")) {
		ChangeScene("Develop");
	}

	ImGui::SeparatorText("Boss");
	if (ImGui::Button("SwitchDebugDraw")) {
		boss_->SwitchDebugDraw();
	}

	ImGui::End();
#endif
	// 平行光源をセット
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// プレイヤーを更新
	player_->Update();
	// ボスを更新
	boss_->Update();

	// 攻撃オブジェクトマネージャ更新
	attackObjectManger_->Update();
	// 攻撃判定更新
	attackCollisionManager_->Update();
	// エフェクトマネージャ更新
	gameEffectManager_->Update();

}

void DevelopScene::Draw() {
	// プレイヤーを描画
	player_->Draw();
	// ボスを描画
	boss_->Draw();

	// 弾マネージャ描画
	attackObjectManger_->Draw();
	// 攻撃判定マネージャ描画
	attackCollisionManager_->Draw();
	// エフェクトマネージャ描画
	gameEffectManager_->Draw();
}

void DevelopScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}
