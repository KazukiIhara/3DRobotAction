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


}

void EffectEditScene::Update() {
	static Vector3 d;
	ImGui::Begin("EffectEditScene");
	ImGui::DragFloat3("Dir", &d.x, 0.01f);

	if (ImGui::Button("Emit")) {
		// 今実装するエフェクト
		LaserEffect::InitParam param{};
		param.emitPos = { 0.0f,0.0f,0.0f };
		param.life = 0.5f;
		param.dir = d;
		std::unique_ptr<LaserEffect> effect = std::make_unique<LaserEffect>(param);
		gameEffectManager_->Add(std::move(effect));
	}
	ImGui::End();

	ImGui::Begin("Plane");
	{
		for (size_t i = 0; i < 4; i++) {
			std::string n = std::to_string(i);
			std::string s = "Plane0" + n;
			ImGui::DragFloat3(s.c_str(), &planeData0_.verticesOffsets[i].x, 0.01f);
		}
	}
	{
		for (size_t i = 0; i < 4; i++) {
			std::string n = std::to_string(i);
			std::string s = "Plane1" + n;
			ImGui::DragFloat3(s.c_str(), &planeData1_.verticesOffsets[i].x, 0.01f);
		}
	}
	ImGui::End();

	gameEffectManager_->Update();

}

void EffectEditScene::Draw() {
	MAGISYSTEM::DrawPlane3D(MakeIdentityMatrix4x4(), planeData0_, MaterialData3D{});
	MAGISYSTEM::DrawPlane3D(MakeIdentityMatrix4x4(), planeData1_, MaterialData3D{});
	gameEffectManager_->Draw();
}

void EffectEditScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}
