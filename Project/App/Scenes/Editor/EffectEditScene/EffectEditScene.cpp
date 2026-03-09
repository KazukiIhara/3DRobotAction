#include "EffectEditScene.h"

// Effects
#include "Feature/Effect/JustDodgeEffect/JustDodgeEffect.h"
#include "Feature/Effect/LaserEffect/LaserEffect.h"
#include "Feature/Effect/BossAttackWarning/BossAttackWarningEffect.h"
#include "Feature/Effect/MachineGunHit/MachineGunHitEffect.h"
#include "Feature/Effect/Explosion/ExplosionEffect.h"
#include "Feature/Effect/BossDestroy/BossDestroyEffect.h"

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
	ImGui::Begin("EffectEditScene");

	if (ImGui::Button("Emit")) {
		// 今実装するエフェクト
		gameEffectManager_->Add(std::make_unique<BossDestroyEffect>(Vector3(0.0f, 0.0f, 0.0f),gameEffectManager_.get()));
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
