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

	// 最初はAI無効
	enemy_->SetIsAIActive(false);

	// プレイヤーのターゲット対象に敵を追加
	player_->GetMechCore().lock()->GetLockOnComponent()->AddMech(enemy_->GetMechCore());
	// エネミーのターゲット対象にプレイヤーを追加
	enemy_->GetMechCore().lock()->GetLockOnComponent()->AddMech(player_->GetMechCore());

	// プレイヤーのボス対象にエネミーを追加
	player_->SetBossMech(enemy_->GetMechCore());


	// 攻撃コリジョンマネージャにワールドに存在するmechを追加
	attackCollisionManager_->AddMech(player_->GetMechCore());
	attackCollisionManager_->AddMech(enemy_->GetMechCore());


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
	// 開始時UIの設定
	// 

	battleUiMatB_.textureName = "StartUI_B.png";
	battleUiMatA_.textureName = "StartUI_A.png";
	battleUiMatT_[0].textureName = "StartUI_T.png";
	battleUiMatT_[1].textureName = "StartUI_T.png";
	battleUiMatL_.textureName = "StartUI_L.png";
	battleUiMatE_.textureName = "StartUI_E.png";

	startUiMatS_.textureName = "StartUI_S.png";
	startUiMatT_[0].textureName = "StartUI_T.png";
	startUiMatA_.textureName = "StartUI_A.png";
	startUiMatR_.textureName = "StartUI_R.png";
	startUiMatT_[1].textureName = "StartUI_T.png";


	battleUiBPosS_ = { 610.0f,-500.0f };
	battleUiAPosS_ = { 720.0f,-700.0f };
	battleUiTPosS_[0] = { 830.0f,-900.0f };
	battleUiTPosS_[1] = { 940.0f,-1100.0f };
	battleUiLPosS_ = { 1050.0f,-1300.0f };
	battleUiEPosS_ = { 1160.0f,-1500.0f };

	startUiSPosS_ = { 665.0f,1180.0f };
	startUiTPosS_[0] = { 775.0f,1380.0f };
	startUiAPosS_ = { 885.0f,1580.0f };
	startUiRPosS_ = { 995.0f,1780.0f };
	startUiTPosS_[1] = { 1105.0f,1980.0f };

	battleUiBPosE_ = { 610.0f,330.0f };
	battleUiAPosE_ = { 720.0f,330.0f };
	battleUiTPosE_[0] = { 830.0f,330.0f };
	battleUiTPosE_[1] = { 940.0f,330.0f };
	battleUiLPosE_ = { 1050.0f,330.0f };
	battleUiEPosE_ = { 1160.0f,330.0f };

	startUiSPosE_ = { 665.0f,490.0f };
	startUiTPosE_[0] = { 775.0f,490.0f };
	startUiAPosE_ = { 885.0f,490.0f };
	startUiRPosE_ = { 995.0f,490.0f };
	startUiTPosE_[1] = { 1105.0f,490.0f };

	battleUiBPosF_ = { 610.0f - 400.0f,-500.0f };
	battleUiAPosF_ = { 720.0f - 200.0f,-500.0f };
	battleUiTPosF_[0] = { 830.0f - 100.0f,-500.0f };
	battleUiTPosF_[1] = { 940.0f + 100.0f,-500.0f };
	battleUiLPosF_ = { 1050.0f + 200.0f,-500.0f };
	battleUiEPosF_ = { 1160.0f + 400.0f,-500.0f };

	startUiSPosF_ = { 665.0f - 400.0f,1180.0f };
	startUiTPosF_[0] = { 775.0f - 200.0f,1180.0f };
	startUiAPosF_ = { 885.0f,1180.0f };
	startUiRPosF_ = { 995.0f + 200.0f,1180.0f };
	startUiTPosF_[1] = { 1105.0f + 400.0f,1180.0f };


	// 開始時アニメーション
	animBattleB_ = SimpleAnimation<Vector2>(battleUiBPosS_, battleUiBPosE_, EasingType::EaseOutQuad);
	animBattleA_ = SimpleAnimation<Vector2>(battleUiAPosS_, battleUiAPosE_, EasingType::EaseOutQuad);
	animBattleL_ = SimpleAnimation<Vector2>(battleUiLPosS_, battleUiLPosE_, EasingType::EaseOutQuad);
	animBattleE_ = SimpleAnimation<Vector2>(battleUiEPosS_, battleUiEPosE_, EasingType::EaseOutQuad);
	for (int i = 0; i < 2; i++) {
		animBattleT_[i] = SimpleAnimation<Vector2>(battleUiTPosS_[i], battleUiTPosE_[i], EasingType::EaseOutQuad);
	}
	animStartS_ = SimpleAnimation<Vector2>(startUiSPosS_, startUiSPosE_, EasingType::EaseOutQuad);
	animStartA_ = SimpleAnimation<Vector2>(startUiAPosS_, startUiAPosE_, EasingType::EaseOutQuad);
	animStartR_ = SimpleAnimation<Vector2>(startUiRPosS_, startUiRPosE_, EasingType::EaseOutQuad);
	for (int i = 0; i < 2; i++) {
		animStartT_[i] = SimpleAnimation<Vector2>(startUiTPosS_[i], startUiTPosE_[i], EasingType::EaseOutQuad);
	}


	// 開始時終了アニメーション
	animFBattleB_ = SimpleAnimation<Vector2>(battleUiBPosE_, battleUiBPosF_, EasingType::EaseOutQuad);
	animFBattleA_ = SimpleAnimation<Vector2>(battleUiAPosE_, battleUiAPosF_, EasingType::EaseOutQuad);
	animFBattleL_ = SimpleAnimation<Vector2>(battleUiLPosE_, battleUiLPosF_, EasingType::EaseOutQuad);
	animFBattleE_ = SimpleAnimation<Vector2>(battleUiEPosE_, battleUiEPosF_, EasingType::EaseOutQuad);
	for (int i = 0; i < 2; ++i) {
		animFBattleT_[i] = SimpleAnimation<Vector2>(battleUiTPosE_[i], battleUiTPosF_[i], EasingType::EaseOutQuad);
	}
	animFStartS_ = SimpleAnimation<Vector2>(startUiSPosE_, startUiSPosF_, EasingType::EaseOutQuad);
	animFStartA_ = SimpleAnimation<Vector2>(startUiAPosE_, startUiAPosF_, EasingType::EaseOutQuad);
	animFStartR_ = SimpleAnimation<Vector2>(startUiRPosE_, startUiRPosF_, EasingType::EaseOutQuad);
	for (int i = 0; i < 2; ++i) {
		animFStartT_[i] = SimpleAnimation<Vector2>(startUiTPosE_[i], startUiTPosF_[i], EasingType::EaseOutQuad);
	}

	//
	// 終了時UIの設定
	//

	finishSpriteData_.position = { WindowApp::kClientWidth * 0.5f,WindowApp::kClientHeight * 0.5f };
	finishSpriteMatData_.anchorPoint = { 0.5f,0.5f };
	finishSpriteMatData_.textureName = "YouWin.png";


	// シーン中のフェーズ処理用変数
	playSceneState_ = PlaySceneState::Start;


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
			if (auto camera = cameraObj->GetCamera3D("Camera")) {
				camera->ApplyCurrent();
			}
		}
	}

	if (ImGui::Button("PlayCameraAnimation")) {
		if (auto cameraObj = MAGISYSTEM::FindGameObject3D("Camera").lock()) {
			if (auto camera = cameraObj->GetCamera3D("Camera")) {
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

	ImGui::Begin("UIPos");

	// Battle UI
	ImGui::Text("Battle UI");
	ImGui::DragFloat2("B_Pos", &battleUiBPos_.x, 1.0f);
	ImGui::DragFloat2("A_Pos", &battleUiAPos_.x, 1.0f);
	ImGui::DragFloat2("L_Pos", &battleUiLPos_.x, 1.0f);
	ImGui::DragFloat2("E_Pos", &battleUiEPos_.x, 1.0f);

	for (int i = 0; i < 2; i++) {
		std::string label = "T_Pos_" + std::to_string(i);
		ImGui::DragFloat2(label.c_str(), &battleUiTPos_[i].x, 1.0f);
	}

	ImGui::Separator();

	// Start UI
	ImGui::Text("Start UI");
	ImGui::DragFloat2("S_Pos", &startUiSPos_.x, 1.0f);
	ImGui::DragFloat2("A_Pos2", &startUiAPos_.x, 1.0f);
	ImGui::DragFloat2("R_Pos", &startUiRPos_.x, 1.0f);

	for (int i = 0; i < 2; i++) {
		std::string label = "Start_T_Pos_" + std::to_string(i);
		ImGui::DragFloat2(label.c_str(), &startUiTPos_[i].x, 1.0f);
	}

	ImGui::End();

#endif

	// ライト変数
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// ポストエフェクト適用
	MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);


	// タイマー更新
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

	// シーンごとの更新処理
	switch (playSceneState_) {
		case PlaySceneState::Start:
		{
			// 敵AI有効
			enemy_->SetIsAIActive(false);
			// プレイヤー操作無効
			player_->SetIsOperation(false);

			// 開始ステートタイマー更新
			startSceneTimer_ += MAGISYSTEM::GetDeltaTime();

			// プレイステートに移行
			if (startSceneTimer_ >= kStartSceneTime_) {
				// 敵AI有効
				enemy_->SetIsAIActive(true);
				// プレイヤー操作有効
				player_->SetIsOperation(true);
				// プレイステートに移行
				playSceneState_ = PlaySceneState::Play;
				break;
			}

			switch (startAnimPhase_) {
				case StartAnimPhase::In:
				{
					// UI更新
					const float t = std::min(startSceneTimer_ / kStartSceneAnimTime_, 1.0f);
					battleUiBPos_ = animBattleB_.GetValue(t);
					battleUiAPos_ = animBattleA_.GetValue(t);
					battleUiLPos_ = animBattleL_.GetValue(t);
					battleUiEPos_ = animBattleE_.GetValue(t);

					for (int i = 0; i < 2; i++) {
						battleUiTPos_[i] = animBattleT_[i].GetValue(t);
					}

					startUiSPos_ = animStartS_.GetValue(t);
					startUiAPos_ = animStartA_.GetValue(t);
					startUiRPos_ = animStartR_.GetValue(t);

					for (int i = 0; i < 2; i++) {
						startUiTPos_[i] = animStartT_[i].GetValue(t);
					}

					if (t >= 1.0f) {
						startAnimPhase_ = StartAnimPhase::Stay;
					}
				}
				break;
				case StartAnimPhase::Stay:
				{
					if (startSceneTimer_ >= 2.0f) {
						startAnimPhase_ = StartAnimPhase::Out;
					}
				}
				break;
				case StartAnimPhase::Out:
				{
					const float t = std::min((startSceneTimer_ - 2.0f) / kStartSceneAnimFTime_, 1.0f);
					battleUiBPos_ = animFBattleB_.GetValue(t);
					battleUiAPos_ = animFBattleA_.GetValue(t);
					battleUiLPos_ = animFBattleL_.GetValue(t);
					battleUiEPos_ = animFBattleE_.GetValue(t);

					for (int i = 0; i < 2; ++i) {
						battleUiTPos_[i] = animFBattleT_[i].GetValue(t);
					}

					// Start UI
					startUiSPos_ = animFStartS_.GetValue(t);
					startUiAPos_ = animFStartA_.GetValue(t);
					startUiRPos_ = animFStartR_.GetValue(t);

					for (int i = 0; i < 2; ++i) {
						startUiTPos_[i] = animFStartT_[i].GetValue(t);
					}
				}
				break;
				default:
					break;
			}

			// Battle UI
			battleUiB_.position = battleUiBPos_;
			battleUiA_.position = battleUiAPos_;
			battleUiL_.position = battleUiLPos_;
			battleUiE_.position = battleUiEPos_;

			for (int i = 0; i < 2; i++) {
				battleUiT_[i].position = battleUiTPos_[i];
			}

			// Start UI
			startUiS_.position = startUiSPos_;
			startUiA_.position = startUiAPos_;
			startUiR_.position = startUiRPos_;

			for (int i = 0; i < 2; i++) {
				startUiT_[i].position = startUiTPos_[i];
			}
		}
		break;
		case PlaySceneState::Play:


			// 勝敗判定
			if (player_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
				info.judge = FinishJudgment::Enemy;
				playSceneState_ = PlaySceneState::Finish;
			}

			if (enemy_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
				info.judge = FinishJudgment::Player;
				playSceneState_ = PlaySceneState::Finish;
			}

			if (player_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0 && enemy_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
				info.judge = FinishJudgment::Draw;
				playSceneState_ = PlaySceneState::Finish;
			}

			// 時間切れの場合
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

				playSceneState_ = PlaySceneState::Finish;
			}

			break;
		case PlaySceneState::Finish:
			// 敵AIを停止
			enemy_->SetIsAIActive(false);

			// 敵勝利時は自機の操作を停止
			if (info.judge == FinishJudgment::Enemy) {
				player_->SetIsOperation(false);
			}

			// 終了シーンタイマー更新
			finishSceneTimer_ -= MAGISYSTEM::GetDeltaTime();

			// シーン終了
			if (finishSceneTimer_ <= 0.0f) {
				ChangeScene("Title");
			}

			break;
	}

}

void PlayScene::Draw() {
	// プレイヤーにまつわるものを描画
	player_->Draw();

	// エネミーにまつわるものを描画
	enemy_->Draw();

	// 弾マネージャ描画
	attackObjectManger_->Draw();

	// 攻撃判定マネージャ描画
	attackCollisionManager_->Draw();

	// ステートごとの描画処理
	switch (playSceneState_) {
		case PlaySceneState::Start:

			MAGISYSTEM::DrawSprite(battleUiB_, battleUiMatB_);
			MAGISYSTEM::DrawSprite(battleUiA_, battleUiMatA_);
			MAGISYSTEM::DrawSprite(battleUiL_, battleUiMatL_);
			MAGISYSTEM::DrawSprite(battleUiE_, battleUiMatE_);

			for (int i = 0; i < 2; ++i) {
				MAGISYSTEM::DrawSprite(battleUiT_[i], battleUiMatT_[i]);
			}

			MAGISYSTEM::DrawSprite(startUiS_, startUiMatS_);
			MAGISYSTEM::DrawSprite(startUiA_, startUiMatA_);
			MAGISYSTEM::DrawSprite(startUiR_, startUiMatR_);

			for (int i = 0; i < 2; ++i) {
				MAGISYSTEM::DrawSprite(startUiT_[i], startUiMatT_[i]);
			}

			break;
		case PlaySceneState::Play:

			break;
		case PlaySceneState::Finish:
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
			break;
	}
}

void PlayScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}