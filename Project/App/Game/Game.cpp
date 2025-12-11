#include "Game.h"
using namespace MAGI;

// EngineScenes
#include "Scenes/Sample/SampleScene.h"

// EditorScene
#include "Scenes/Editor/MechEditScene/MechEditScene.h"

// GameScene
#include "Scenes/Game/Title/TitleScene.h"
#include "Scenes/Game/Menu/MenuScene.h"
#include "Scenes/Game/Play/PlayScene.h"
#include "Scenes/Game/MechCustomScene/MechCustomScene.h"

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
