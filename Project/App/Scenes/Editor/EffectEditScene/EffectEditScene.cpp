#include "EffectEditScene.h"

// Effects
#include "Feature/Effect/JustDodgeEffect/JustDodgeEffect.h"
#include "Feature/Effect/LaserEffect/LaserEffect.h"

void EffectEditScene::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// 3Dカメラ作成
	std::unique_ptr<Camera3D> sceneCamera3D = std::make_unique<Camera3D>("SceneCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera3D))->ApplyCurrent();

	// エフェクトマネージャ初期化
	gameEffectManager_ = std::make_unique<GameEffectManager>();


	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));


	// 床追加
	MAGISYSTEM::LoadSceneDataFromJson("SceneData");
	MAGISYSTEM::ImportSceneData("SceneData", true);

}

void EffectEditScene::Update() {
	ImGui::Begin("EffectEditScene");
	if (ImGui::Button("Emit")) {
		// 今実装するエフェクト

		LaserEffect::InitParam param{};
		param.emitPos = { 0.0f,0.0f,0.0f };
		param.dir = { 0.0f,0.0f,1.0f };
		param.speed = 100.0f;
		param.life = 0.5f;

		std::unique_ptr<LaserEffect> effect = std::make_unique<LaserEffect>(param);
		gameEffectManager_->Add(std::move(effect));
	}
	ImGui::End();

	gameEffectManager_->Update();

}

void EffectEditScene::Draw() {
	gameEffectManager_->Draw();
}

void EffectEditScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}
