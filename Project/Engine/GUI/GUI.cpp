#include "GUI.h"

#include <cassert>


#include "Logger/Logger.h"

#include "DeltaTimer/DeltaTimer.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

// Windows環境限定
#ifdef _WIN32
#include <Windows.h>
#endif
GUI::GUI(DeltaTimer* deltaTimer, SRVUAVManager* srvUavManager) {
	// 
	// インスタンスのセット
	// 
	assert(deltaTimer);
	assert(srvUavManager);
	deltaTimer_ = deltaTimer;
	srvUavManager_ = srvUavManager;

	// 
	// データIOクラスから受け取るインスタンス
	// 


}

void GUI::ShowFPS() {
	// フレームレート表示の設定
	ImGuiIO& io = ImGui::GetIO();

	// 左上に固定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowBgAlpha(0.0f); // 背景を透明にする

	// ウィンドウを作成せずに、文字だけを描画
	ImGui::Begin("FPS Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	// 文字色を緑に変更
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // 緑色
	ImGui::Text("FPS: %.1f", io.Framerate);
	ImGui::PopStyleColor();

	ImGui::End();
}

void GUI::ShowDeltaTime() {
	// FPS表示の少し下に固定
	ImGui::SetNextWindowPos(ImVec2(0, 16));
	ImGui::SetNextWindowBgAlpha(0.0f); // 背景を透明にする

	// ウィンドウを作成せずに、文字だけを描画
	ImGui::Begin("DeltaTime Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	// 文字色を緑に変更
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // 緑色
	ImGui::Text("DeltaTime: %.3f", deltaTimer_->GetDeltaTime());
	ImGui::PopStyleColor();

	ImGui::End();
}

void GUI::ShowMainTab() {
	// メインメニューバー作成
	if (ImGui::BeginMainMenuBar()) {

		// ファイルの処理
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene")) { /* 新しいシーンの処理 */ }
			if (ImGui::MenuItem("Open Scene")) { /* シーンを開く処理 */ }
			if (ImGui::MenuItem("Save Scene")) { /* シーンを保存する処理 */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) { /* アプリ終了処理 */ }
			ImGui::EndMenu();
		}

		// オブジェクトの処理
		if (ImGui::BeginMenu("Object")) {
			// Object3D
			if (ImGui::BeginMenu("GameObject3D")) {


				ImGui::EndMenu();
			}

			// Renderer3D
			if (ImGui::BeginMenu("Renderer3D")) {


				ImGui::EndMenu();
			}

			// ParticleGroup
			if (ImGui::BeginMenu("ParticleGroup3D")) {


				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void GUI::ShowSceneWindow() {
	// ウィンドウの装飾（パディングやタイトルバー）を考慮
	ImVec2 padding = ImGui::GetStyle().WindowPadding; // ウィンドウの左右パディング
	float titleBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2; // タイトルバーの高さ

	// ウィンドウサイズを設定
	ImVec2 windowSize = { sceneTextureSize_.x + padding.x * 2, sceneTextureSize_.y + padding.y * 2 + titleBarHeight };
	ImGui::SetNextWindowSize(windowSize);

	// シーンウィンドウ開始
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoResize);

	//ImGui::ImageWithBg(static_cast<ImTextureID>(srvUavManager_->GetDescriptorHandleGPU(srvIndex).ptr), sceneTextureSize_, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	ImGui::End();
}

void GUI::ShowHierarchyWindow() {
	ImGui::Begin("Hierarchy");

	ImGui::End();
}

void GUI::ShowInspectorWindow() {
	ImGui::Begin("Inspector");

	ImGui::End();
}

void GUI::ShowProjectWindow() {
	ImGui::Begin("Project");

	ImGui::End();
}

void GUI::ShowMainUI() {
	// メインタブ
	ShowMainTab();
	// シーンウィンドウ
	ShowSceneWindow();
	// ヒエラルキーウィンドウ
	ShowHierarchyWindow();
	// インスペクターウィンドウ
	ShowInspectorWindow();
	// プロジェクトウィンドウを描画
	ShowProjectWindow();
}

void GUI::ShowDebugUI() {
	ShowFPS();
	ShowDeltaTime();
}

bool& GUI::GetIsShowMainUI() {
	return isShowMainUI_;
}