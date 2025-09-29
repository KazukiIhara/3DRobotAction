#include "PlayScene.h"

void PlayScene::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));

	//-------------------------------------------------------
	// アセットのロード
	//-------------------------------------------------------
	uint32_t skyBoxTexutreIndex = MAGISYSTEM::LoadTexture("kloppenheim_06_puresky_2k.dds");


	//-------------------------------------------------------
	// シーン固有の初期化処理
	//-------------------------------------------------------

	// スカイボックスを設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	//===========================
	// マネージャの初期化
	//===========================

	// 攻撃コリジョンマネージャ
	attackCollisionManager_ = std::make_unique<AttackCollisionManager>();

	// 弾マネージャ
	attackObjectManger_ = std::make_unique<AttackObjectManager>(attackCollisionManager_.get());

	// プレイヤー作成
	player_ = std::make_unique<Player>(attackObjectManger_.get());

	// 敵作成
	enemy_ = std::make_unique<Enemy>(attackObjectManger_.get(), player_->GetMechCore());

	// プレイヤーのターゲット対象に敵を追加
	player_->GetMechCore().lock()->GetLockOnComponent()->AddMech(enemy_->GetMechCore());
	// エネミーのターゲット対象にプレイヤーを追加
	enemy_->GetMechCore().lock()->GetLockOnComponent()->AddMech(player_->GetMechCore());

	// プレイヤーのボス対象にエネミーを追加
	player_->SetBossMech(enemy_->GetMechCore());


	// 攻撃コリジョンマネージャにワールドに存在するmechを追加
	attackCollisionManager_->AddMech(player_->GetMechCore());
	attackCollisionManager_->AddMech(enemy_->GetMechCore());

	//===========================
	// 以下ほぼほぼデバッグ用
	//===========================


	planeEffect_.Initialize();
	planeEffect_.blendMode = BlendMode::Normal;
	planeEffect_.translate.isAnimated = true;
	planeEffect_.translate.startTime = 0.0f;
	planeEffect_.translate.animation.SetEasing(EasingType::EaseInCubic);
	planeEffect_.translate.animation.SetEnd(Vector3(0.0f, 0.5f, 0.0f));
	planeEffect_.color.isAnimated = true;
	planeEffect_.color.animation.SetEnd(Vector4(1.0f, 1.0f, 1.0f, 0.0f));


	//-------------------------------------------------------
	// シーンデータのロード
	//-------------------------------------------------------

	MAGISYSTEM::LoadSceneDataFromJson("SceneData");

	MAGISYSTEM::ImportSceneData("SceneData", true);

	//
	// タイマーリセット
	// 
	tempBattleTime_ = 0.0f;
	info.battleTime = kMaxBattleTime_;

	//
	// 終了時UIの設定
	//

	finishSpriteData_.position = { WindowApp::kClientWidth * 0.5f,WindowApp::kClientHeight * 0.5f };
	finishSpriteMatData_.anchorPoint = { 0.5f,0.5f };
	finishSpriteMatData_.textureName = "YouWin.png";

}

void PlayScene::Update() {

#if defined(DEBUG) || defined(DEVELOP)
	ImGui::Begin("SceneDebugUI");
	ImGui::Text("BattleTime:%u", info.battleTime);

	if (ImGui::Button("ImportScene")) {
		MAGISYSTEM::LoadSceneDataFromJson("SceneData1");
		MAGISYSTEM::ImportSceneData("SceneData1", true);
	}

	if (ImGui::Button("ApplyImportCamera")) {
		if (auto cameraObj = MAGISYSTEM::FindGameObject3D("Camera").lock()) {
			if (auto camera = cameraObj->GetCamera3D("Camera").lock()) {
				camera->ApplyCurrent();
			}
		}
	}

	if (ImGui::Button("PlayCameraAnimation")) {
		if (auto cameraObj = MAGISYSTEM::FindGameObject3D("Camera").lock()) {
			if (auto camera = cameraObj->GetCamera3D("Camera").lock()) {
				camera->ApplyCurrent();
				camera->StartEyeAnimation();
				camera->StartTargetAnimation();
			}
		}
	}

	bool isActiveEnemyAI = enemy_->GetIsAIActive();
	if (ImGui::Checkbox("EnableEnemyAI", &isActiveEnemyAI)) {
		enemy_->SetIsAIActive(isActiveEnemyAI);
	}

	ImGui::End();
#endif

	// ライト変数
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// ポストエフェクト適用
	MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectDepthNormalOutline();


	// 
	// タイマー更新
	// 

	tempBattleTime_ += MAGISYSTEM::GetDeltaTime();

	// 一秒経ったらタイマーをマイナス
	if (tempBattleTime_ >= 1.0f) {
		tempBattleTime_ = 0.0f;
		if (info.battleTime > 0) {
			info.battleTime--;
		}
	}

	//
	// オブジェクト更新
	//

	// プレイヤー更新
	player_->Update();

	// 敵更新
	enemy_->Update();

	// 弾マネージャ更新
	attackObjectManger_->Update();

	// 攻撃判定更新
	attackCollisionManager_->Update();

	// 
	// シーン終了判定
	// 

	if (player_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
		info.judge = FinishJudgment::Enemy;
		info.finish = true;
	}

	if (enemy_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
		info.judge = FinishJudgment::Player;
		info.finish = true;
	}

	if (player_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0 && enemy_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
		info.judge = FinishJudgment::Draw;
		info.finish = true;
	}

	if (info.battleTime == 0) {
		// 体力割合が多いほうが勝利
		float playerHPRaito = player_->GetMechCore().lock()->GetStatusComponent()->GetHPRaito();
		float enemyHPraito = enemy_->GetMechCore().lock()->GetStatusComponent()->GetHPRaito();

		if (playerHPRaito > enemyHPraito) {
			info.judge = FinishJudgment::Player;
		} else if (enemyHPraito < playerHPRaito) {
			info.judge = FinishJudgment::Enemy;
		} else {
			info.judge = FinishJudgment::Draw;
		}

		info.finish = true;
	}

	if (info.finish) {
		enemy_->SetIsAIActive(false);
		finishSceneTimer_ -= MAGISYSTEM::GetDeltaTime();
	}

	if (finishSceneTimer_ <= 0.0f) {
		ChangeScene("Title");
	}

}

void PlayScene::Draw() {
	// プレイヤーにまつわるもの描画
	player_->Draw();

	// 弾マネージャ描画
	attackObjectManger_->Draw();

	// 攻撃判定マネージャ描画
	attackCollisionManager_->Draw();

	// 戦闘終了時の描画
	if (info.finish) {
		switch (info.judge) {
			case FinishJudgment::Player:
				finishSpriteMatData_.textureName = "YouWin.png";
				MAGISYSTEM::DrawSprite(finishSpriteData_, finishSpriteMatData_);
				break;
			case FinishJudgment::Enemy:
				finishSpriteMatData_.textureName = "YouLose.png";
				MAGISYSTEM::DrawSprite(finishSpriteData_, finishSpriteMatData_);
				break;
			case FinishJudgment::Draw:
				finishSpriteMatData_.textureName = "YouWin.png";
				MAGISYSTEM::DrawSprite(finishSpriteData_, finishSpriteMatData_);
				break;
			default:
				break;
		}
	}

}

void PlayScene::Finalize() {
	MAGISYSTEM::ClearGameObject3D();
}