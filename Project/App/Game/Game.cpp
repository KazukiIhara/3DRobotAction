#include "Game.h"

// EngineScenes
#include "Scenes/SampleScene.h"
#include "Scenes/ParticleEditScene.h"

Game::~Game() {}

void Game::Initialize() {
	// 規定システムの初期化
	MAGISYSTEM::Initialize();

	// シーンの追加
	// サンプルシーン
	sceneManager_->AddScene<SampleScene>("Sample");
	// パーティクル作成シーン
	sceneManager_->AddScene<ParticleEditScene>("ParticleEdit");

	// 最初のシーンを設定
	sceneManager_->ChangeScene("Sample");
}
