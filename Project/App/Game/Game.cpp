#include "Game.h"

// EngineScenes
#include "Scenes/Sample/SampleScene.h"

// EditorScene
#include "Scenes/Editor/EffectEditScene/EffectEditScene.h"

// GameScene
#include "Scenes/Game/Develop/DevelopScene.h"

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

	// エフェクト作成シーン
	sceneManager_->AddScene<EffectEditScene>("EffectEdit");

	// 開発シーン
	sceneManager_->AddScene<DevelopScene>("Develop");

	// 最初のシーンを設定
	sceneManager_->ChangeScene("Develop");
}