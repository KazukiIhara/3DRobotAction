#pragma once

#define NOMINMAX

#include <cstdint>
#include <Windows.h>

// ウィンドウ管理クラス
class WindowApp {
public:
	// クライアント領域
	static const uint32_t kClientWidth = 1920; // クライアント領域の横幅
	static const uint32_t kClientHeight = 1080; // クライアント領域の縦幅
public:
	WindowApp();
	~WindowApp();
	// 初期化
	void Initialize();
	// 終了処理
	void Finalize();
	// 更新
	bool Update();
	// フルスクリーンとウィンドウモードを切り替える
	void ToggleFullScreen();

	// ウィンドウハンドルの取得
	HWND GetHwnd() const;
	// ウィンドウクラスの取得
	WNDCLASS GetWndClass() const;
private:
	// ゲームウィンドウの作成
	void CreateGameWindow(const wchar_t* title = L"MAGI with DirectX12", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kClientWidth, int32_t clientHeight = kClientHeight);
	// メッセージの処理
	bool ProcessMessage();
	// ゲームウィンドウの破棄
	void TerminateGameWindow();
public: // 静的メンバ関数
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	// ウィンドウハンドル
	HWND hwnd_ = nullptr;
	// ウィンドウクラス
	WNDCLASS wc_{};
	// フルスクリーンかどうかを保持するフラグ
	bool isFullScreen_ = false;
	// ウィンドウモードの復元用の矩形
	RECT windowRect_ = { 0, 0, kClientWidth, kClientHeight };
};