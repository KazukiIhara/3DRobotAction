#include "DevelopScene.h"

using namespace Magi;

void DevelopScene::Initialize() {
	// 3Dカメラ作成
	std::unique_ptr<TPSCamera3D> sceneCamera3D = std::make_unique<TPSCamera3D>("SceneCamera3D");
	// マネージャに追加
	Camera3D* camera = MAGISYSTEM::AddCamera3D(std::move(sceneCamera3D));

	// TPSカメラにキャスト
	camera_ = dynamic_cast<TPSCamera3D*>(camera);
	// 現在のカメラに設定
	camera_->ApplyCurrent();

	// ライトを設定
	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// リソースロード
	LoadResource();

	//===========================
	// 操作クラスの初期化
	//===========================
	inputSys_ = std::make_unique<GameInputSystem>();

	//===========================
	// ステージの初期化
	//===========================

	stageData_ = std::make_unique<CombatStageData>();

	// ステージデータをロード
	stageData_->LoadJson("Assets/StageData/CombatStage.json");

	//===========================
	// マネージャの初期化
	//===========================

	// ゲームエフェクトマネージャ
	gameEffectManager_ = std::make_unique<GameEffectManager>();
	// 攻撃オブジェクトマネージャ
	damageObjectManager_ = std::make_unique<DamageObjectManager>();
	// コリジョンマネージャ
	damageCollisionSystem_ = std::make_unique<DamageCollisionSystem>();
	// 機体アニメーションコンテナクラス
	mechAnimationContainer_ = std::make_unique<MechAnimationContainer>();


	//===========================
	// エディターの初期化
	//===========================

	// 機体アニメーション作成クラス
	mechAnimationEdit_ = std::make_unique<MechAnimationEdit>(mechAnimationContainer_.get());
	// ステージ作成クラス
	stageEditor_ = std::make_unique<CombatStageEditor>(stageData_.get());

	// 機体の作成に必要なシステムの参照ポインタ
	BaseMech::RefContext ref{
		damageObjectManager_.get(),
		gameEffectManager_.get(),
		damageCollisionSystem_.get(),
		mechAnimationContainer_.get(),
		stageData_.get()
	};

	// パイロット作成に必要な参照ポインタ配列
	Pilot::RefContext pref{
		camera_,
		inputSys_.get()
	};

	// パイロット
	pilot_ = std::make_unique<Pilot>(ref, pref);

	// ボス作成
	boss_ = std::make_unique<Boss>(ref, pilot_->GetMech());


	// 暫定処理　現在はボスしかロックオン対象がいないため、直接セット
	pilot_->GetMech()->GetLockOnSystem()->SetBoss(boss_->GetMech());


	// 攻撃判定マネージャに機体を追加
	damageCollisionSystem_->AddMech(pilot_->GetMech());
	damageCollisionSystem_->AddMech(boss_->GetMech());


	// シーンコントローラ作成
	CombatSceneControl::ContextRef cscRef = {
		inputSys_.get(),
		camera_,
		pilot_.get(),
		boss_.get(),
		gameEffectManager_.get(),
		damageCollisionSystem_.get(),
		damageObjectManager_.get(),
	};
	sceneController_ = std::make_unique<CombatSceneController>(cscRef);


	// アニメーション作成クラスにボスをセット
	mechAnimationEdit_->SetBaseMech(pilot_->GetMech());

}

void DevelopScene::Update() {

#if defined(DEBUG)|(DEVELOP)
	// デバッグ用途の処理
	ImGui::Begin("DevelopUI");
	if (ImGui::Button("Play")) {
		ChangeScene("Title");
	}
	ImGui::SeparatorText("Scene");
	if (ImGui::Button("ResetScene")) {
		ChangeScene("Develop");
	}
	ImGui::SameLine();
	if (ImGui::Button("EffectEditScene")) {
		ChangeScene("EffectEdit");
	}
	ImGui::SameLine();
	if (ImGui::Button("StartCombat")) {
		sceneController_->Start(CombatSceneController::State::Start);
	}

	ImGui::Text("CurrentSceneState: ");
	ImGui::SameLine();
	ImGui::Text(sceneController_->GetCurrentStateStr().c_str());

	ImGui::SeparatorText("Pilot");
	if (ImGui::Button("SwitchPilotControl")) {
		inputSys_->SwitchPilotControl();
	}
	if (ImGui::Button("PilotMechDebugDraw")) {
		pilot_->SwitchDebugDraw();
	}
	ImGui::SeparatorText("Boss");
	if (ImGui::Button("BossMechDebugDraw")) {
		boss_->SwitchDebugDraw();
	}
	if (ImGui::Button("ActiveAI")) {
		boss_->SwitchAIActive();
	}
	ImGui::SeparatorText("Stage");
	if (ImGui::Button("StageDebugDraw")) {
		stageData_->SwitchShowImGui();
	}

	ImGui::SeparatorText("CollisionSystem");
	if (ImGui::Button("DamageCollisionDraw")) {
		damageCollisionSystem_->SwitchIsDebugDraw();
	}

	ImGui::End();

	// 機体アニメーション作成クラス
	mechAnimationEdit_->Update();

	// ステージデータ作成クラス
	stageEditor_->DrawImGui();
#endif

	// 平行光源をセット
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// コマンドを更新
	inputSys_->Update();

	// シーン管理クラス更新
	const bool endRequest = sceneController_->Update();

	// シーン終了
	if (endRequest) {
		ChangeScene("Develop");
	}

	// パイロットを更新
	pilot_->Update();

	// ボスを更新
	boss_->Update();

	// 攻撃オブジェクトマネージャ更新
	damageObjectManager_->Update();

	// 攻撃判定システム更新
	damageCollisionSystem_->Update();

	// エフェクトマネージャ更新
	gameEffectManager_->Update();

}

void DevelopScene::Draw() {
	// ステージ描画
	stageData_->Draw();

	// シーン管理クラス描画
	sceneController_->Draw();

	// パイロットを更新
	pilot_->Draw();

	// ボスを描画
	boss_->Draw();

	// 攻撃オブジェクトマネージャ描画
	damageObjectManager_->Draw();

	// 攻撃判定システム描画
	damageCollisionSystem_->Draw();

	// エフェクトマネージャ描画
	gameEffectManager_->Draw();


	MAGISYSTEM::DrawFont("AAABBBbbb gty", Vector2(0.0f, 100.0f), Color::White, 0.25f);

	MAGISYSTEM::DrawFont("AAABBBbbb gty", Vector2(0.0f, 200.0f), Color::White, 1.0f);
}

void DevelopScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}

void DevelopScene::LoadResource() {
	// テクスチャ
	MAGISYSTEM::LoadTexture("white.png");
	MAGISYSTEM::LoadTexture("gradation.png");
	MAGISYSTEM::LoadTexture("dodgeEffect.png");
	MAGISYSTEM::LoadTexture("Circle.png");
	MAGISYSTEM::LoadTexture("Circle2.png");
	MAGISYSTEM::LoadTexture("electric_0.png");
	MAGISYSTEM::LoadTexture("lensFlare.png");
	MAGISYSTEM::LoadTexture("gradationToon.png");


	MAGISYSTEM::LoadTexture("YellowBullet.png");
	MAGISYSTEM::LoadTexture("Laser2.png");
}
