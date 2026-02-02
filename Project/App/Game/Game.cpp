#include "Game.h"

// EngineScenes
#include "Scenes/Sample/SampleScene.h"

// EditorScene
#include "Scenes/Editor/EffectEditScene/EffectEditScene.h"

// GameScene
#include "Scenes/Game/Develop/DevelopScene.h"

#include "Scenes/Game/Title/TitleScene.h"
#include "Scenes/Game/Menu/MenuScene.h"
#include "Scenes/Game/Combat/CombatScene.h"
#include "Scenes/Game/Result/ResultScene.h"

Game::~Game() {}

/// <summary>
/// アプリ実行クラス
/// </summary>
void Game::Initialize() {
	// 規定システムの初期化
	MAGISYSTEM::Initialize();

	// サウンドのロード
	LoadSound();

	// シーンの追加
	// サンプルシーン
	sceneManager_->AddScene<SampleScene>("Sample");

	// エフェクト作成シーン
	sceneManager_->AddScene<EffectEditScene>("EffectEdit");

	// 開発シーン
	sceneManager_->AddScene<DevelopScene>("Develop");

	// タイトルシーン
	sceneManager_->AddScene<TitleScene>("Title");
	// メニューシーン
	sceneManager_->AddScene<MenuScene>("Menu");
	// 戦闘シーン
	sceneManager_->AddScene<CombatScene>("Combat");
	// リザルトシーン
	sceneManager_->AddScene<ResultScene>("Result");

#if defined(DEVELOP)|(DEBUG)
	// 最初のシーンを設定
	sceneManager_->StartScene("Develop");
#else
	sceneManager_->StartScene("Title");
#endif
}

void Game::LoadSound() {
	MAGISYSTEM::LoadWaveSound("BossAttackWarning.wav");
	MAGISYSTEM::LoadWaveSound("JustDodge.wav");
	MAGISYSTEM::LoadWaveSound("JustDodge_Mid.wav");
}
