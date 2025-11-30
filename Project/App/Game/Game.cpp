#include "Game.h"

// EngineScenes
#include "Scenes/Sample/SampleScene.h"

// EditorScene
#include "Scenes/MechEditScene/MechEditScene.h"

// GameScene
#include "Scenes/Title/TitleScene.h"
#include "Scenes/Play/PlayScene.h"

Game::~Game() {}

/// <summary>
/// アプリ実行クラス
/// </summary>
void Game::Initialize() {
	// 規定システムの初期化
	MAGISYSTEM::Initialize();

	// シーンの追加
	// サンプルシーン
	sceneManager_->AddScene<SampleScene>("Sample");

	// エディットシーン
	sceneManager_->AddScene<MechEditScene>("MechEdit");

	// ゲームシーン
	sceneManager_->AddScene<TitleScene>("Title");
	sceneManager_->AddScene<PlayScene>("Play");


	// 最初のシーンを設定
	sceneManager_->ChangeScene("Title");
}