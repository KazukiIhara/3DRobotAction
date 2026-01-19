#include "DevelopScene.h"

#include "GameEffects/LaserEffect/LaserEffect.h"

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

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));


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

	// ボス作成
	BaseMech::RefContext ref{
		damageObjectManager_.get(), gameEffectManager_.get(), mechAnimationContainer_.get()
	};


	boss_ = std::make_unique<Boss>(ref);

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
		if (ImGui::Button("SwitchDebugDraw")) {
			boss_->SwitchDebugDraw();
		}

		ImGui::End();

		// 機体アニメーション作成クラス
		mechAnimationEdit_->Update();
	}

	// 平行光源をセット
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// ボスを更新
	boss_->Update();

	// 攻撃オブジェクトマネージャ更新
	damageObjectManager_->Update();

	// エフェクトマネージャ更新
	gameEffectManager_->Update();

}

void DevelopScene::Draw() {
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
