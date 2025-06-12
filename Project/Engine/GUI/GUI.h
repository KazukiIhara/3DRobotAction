#pragma once

// C++
#include <vector>
#include <memory>

// ImGui
#include "imgui/imgui.h"

// 前方宣言
class DeltaTimer;
class SRVUAVManager;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(
		DeltaTimer* deltaTimer,
		SRVUAVManager* srvUavManager
	);

	// メインUI描画
	void ShowMainUI();

	// デバッグ表示
	void ShowDebugUI();

	//
	// アクセッサ
	//

	// エンジンウィンドウの表示フラグ
	bool& GetIsShowMainUI();

private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();

	// メインタブ描画
	void ShowMainTab();

	// シーンウィンドウ描画
	void ShowSceneWindow();

	// ヒエラルキー描画
	void ShowHierarchyWindow();

	// インスペクター描画
	void ShowInspectorWindow();

	// プロジェクトファイル描画
	void ShowProjectWindow();

private:
	// エンジンのウィンドウを描画するフラグ
	bool isShowMainUI_ = false;

private:
	// 
	// UI変数
	// 

	// ヒエラルキーウィンドウ
	ImVec2 hierarchyWindowPosition_ = { 0.0f,64.0f };

	// シーンウィンドウ
	ImVec2 sceneTextureSize_ = { 1024.0f,576.0f };

private:
	// DeltaTimerのインスタンス
	DeltaTimer* deltaTimer_ = nullptr;
	// SrvUavManagerのインスタンス
	SRVUAVManager* srvUavManager_ = nullptr;
};
