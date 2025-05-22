#pragma once

#include <cstdint>

#include "WindowApp/WindowApp.h"

class DirectXCommand;

class Viewport {
public:
	Viewport(DirectXCommand* directXCommand);
	~Viewport();

	// 初期化
	void Initialize(DirectXCommand* directXCommand);

	// ビューポートを設定
	void SettingViewport(uint32_t width = WindowApp::kClientWidth, uint32_t height = WindowApp::kClientHeight);

private:
	// コマンドのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* directXCommand);

private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
};