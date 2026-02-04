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

	// フォントのロード
	LoadFonts();

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

void Game::LoadFonts() {
	// フォント読み込み、テクスチャ作成
	const bool ok = MAGISYSTEM::BuildAsciiAtlasPng(
		"Assets/Fonts/ttf/Roboto-Bold.ttf",
		"Assets/Fonts/Roboto_Bold.png",
		256,
		512,
		16
	);
	MAGIAssert::Assert(ok, "FontAtlas生成に失敗しました");

	// フォント描画用データを書き出し
	const bool saveOK = MAGISYSTEM::SaveGlyphJson("Assets/Fonts/Roboto_Bold.json");

	MAGIAssert::Assert(saveOK, "FontAtlas生成に失敗しました");

	// テクスチャロード
	MAGISYSTEM::LoadTexture("Assets/Fonts/Roboto_Bold.png", true);

	// フォント描画クラスでフォントを読み込み
	MAGISYSTEM::LoadFont("Assets/Fonts/Roboto_Bold.png", "Assets/Fonts/Roboto_Bold.json");
}
