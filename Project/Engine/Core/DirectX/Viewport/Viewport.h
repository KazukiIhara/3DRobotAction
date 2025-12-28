#pragma once

#include <cstdint>

#include "WindowApp/WindowApp.h"

namespace Magi {
	class DirectXCommand;

	class Viewport {
	public:
		Viewport(DirectXCommand* directXCommand);
		~Viewport();

		// 初期化
		void Initialize(DirectXCommand* directXCommand);

		// ビューポートを設定
		void SettingViewport(uint32_t width = Magi::WindowApp::kClientWidth, uint32_t height = Magi::WindowApp::kClientHeight);

	private:
		// コマンドのインスタンスをセット
		void SetDirectXCommand(DirectXCommand* directXCommand);

	private:
		// コマンドのインスタンスを受け取る箱
		DirectXCommand* directXCommand_ = nullptr;
	};
}
