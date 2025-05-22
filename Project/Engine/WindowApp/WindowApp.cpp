#include "WindowApp.h"

#include <cassert>

#include "Logger/Logger.h"
#include "ImGuiController/ImGuiController.h"

WindowApp::WindowApp() {
	Initialize();
	Logger::Log("WindowApp Initialize\n");
}

WindowApp::~WindowApp() {
	Finalize();
	Logger::Log("WindowApp Finalize\n");
}

void WindowApp::Initialize() {
	// システムタイマー分解能をあげる
	timeBeginPeriod(1);
	// ゲームウィンドウの作成
	CreateGameWindow();
}

void WindowApp::Finalize() {
	// ウィンドウを閉じる
	CloseWindow(hwnd_);
	// ウィンドウを破棄
	TerminateGameWindow();
}

bool WindowApp::Update() {
	return ProcessMessage();
}

bool WindowApp::ProcessMessage() {
	// メッセージ
	MSG msg{};

	// ウィンドウにメッセージが来ていたら最優先で処理
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ウィンドウのxボタンが押されるまでループ
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WindowApp::ToggleFullScreen() {
	// 現在のフルスクリーン状態を確認
	if (!isFullScreen_) {
		// ウィンドウモードからフルスクリーンに変更

		// 現在のウィンドウの位置とサイズを保存
		GetWindowRect(hwnd_, &windowRect_);

		// ディスプレイの解像度を取得
		uint32_t screenWidth = GetSystemMetrics(SM_CXSCREEN);
		uint32_t screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// フルスクリーンに設定
		SetWindowLongPtr(hwnd_, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(hwnd_, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
		ShowWindow(hwnd_, SW_MAXIMIZE);

		// フルスクリーンフラグを設定
		isFullScreen_ = true;
	} else {
		// フルスクリーンからウィンドウモードに変更

		// ウィンドウスタイルを元に戻す
		SetWindowLongPtr(hwnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		SetWindowPos(hwnd_, HWND_TOP, windowRect_.left, windowRect_.top,
			windowRect_.right - windowRect_.left, windowRect_.bottom - windowRect_.top, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
		ShowWindow(hwnd_, SW_RESTORE);

		// フルスクリーンフラグを解除
		isFullScreen_ = false;
	}
}

HWND WindowApp::GetHwnd() const {
	return hwnd_;
}

WNDCLASS WindowApp::GetWndClass() const {
	return wc_;
}

void WindowApp::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight) {
	// COMの初期化
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	SUCCEEDED(hr);

	// ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc_.lpszClassName = title;
	// インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc_);

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc{ 0,0,clientWidth,clientHeight };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, windowStyle, false);

	// ウィンドウの生成
	hwnd_ = CreateWindow
	(
		wc_.lpszClassName,		// 利用するクラス名
		title,					// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// よく見るウィンドウスタイル
		CW_USEDEFAULT,			// 表示X座標
		CW_USEDEFAULT,			// 表示Y座標
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wc_.hInstance,			// インスタンスハンドル
		nullptr					// オプション
	);

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

void WindowApp::TerminateGameWindow() {
	// ウィンドウクラスを登録解除
	UnregisterClass(wc_.lpszClassName, wc_.hInstance);
	// COM 終了
	CoUninitialize();
}

LRESULT WindowApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// ImGuiでもマウス操作ができるようになるやつ
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
