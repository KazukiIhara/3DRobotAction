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

	// 
	// リソースロード
	//

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));


	//===========================
	// 操作クラスの初期化
	//===========================
	inputSys_ = std::make_unique<GameInputSystem>();


	//===========================
	// マネージャの初期化
	//===========================

	// ゲームエフェクトマネージャ
	gameEffectManager_ = std::make_unique<GameEffectManager>();
	// コリジョンマネージャ
	damageCollisionSystem_ = std::make_unique<DamageCollisionSystem>();
	// 攻撃オブジェクトマネージャ
	damageObjectManager_ = std::make_unique<DamageObjectManager>(damageCollisionSystem_.get());
	// 機体アニメーションコンテナクラス
	mechAnimationContainer_ = std::make_unique<MechAnimationContainer>();
	// 機体アニメーション作成クラス
	mechAnimationEdit_ = std::make_unique<MechAnimationEdit>(mechAnimationContainer_.get());

	// 機体の作成に必要なシステムの参照ポインタ
	BaseMech::RefContext ref{
		damageObjectManager_.get(), gameEffectManager_.get(), mechAnimationContainer_.get()
	};

	// パイロット作成に必要な参照ポインタ配列
	Pilot::RefContext pref{
		camera_, inputSys_.get()
	};

	// パイロット
	pilot_ = std::make_unique<Pilot>(ref, pref);

	// ボス作成
	boss_ = std::make_unique<Boss>(ref,pilot_->GetMech());

	// アニメーション作成クラスにボスをセット
	mechAnimationEdit_->SetBaseMech(boss_->GetMech());

	// 床追加
	MAGISYSTEM::LoadSceneDataFromJson("SceneData");
	MAGISYSTEM::ImportSceneData("SceneData", true);

}

void DevelopScene::Update() {
	// デバッグ用途の処理
	{
		ImGui::Begin("DevelopUI");

		ImGui::SeparatorText("Scene");
		if (ImGui::Button("ResetScene")) {
			ChangeScene("Develop");
		}

		ImGui::SeparatorText("Boss");
		if (ImGui::Button("BossDebugDraw")) {
			boss_->SwitchDebugDraw();
		}

		ImGui::SeparatorText("Pilot");
		if (ImGui::Button("PilotDebugDraw")) {
			pilot_->SwitchDebugDraw();
		}
		ImGui::End();

		// 機体アニメーション作成クラス
		mechAnimationEdit_->Update();
	}

	// 平行光源をセット
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// コマンドを更新
	inputSys_->Update();

	// パイロットを更新
	pilot_->Update();

	// ボスを更新
	boss_->Update();

	// 攻撃オブジェクトマネージャ更新
	damageObjectManager_->Update();

	// エフェクトマネージャ更新
	gameEffectManager_->Update();

}

void DevelopScene::Draw() {
	// パイロットを更新
	pilot_->Draw();

	// ボスを描画
	boss_->Draw();

	// 攻撃オブジェクトマネージャ描画
	damageObjectManager_->Draw();

	// エフェクトマネージャ描画
	gameEffectManager_->Draw();
}

void DevelopScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}
