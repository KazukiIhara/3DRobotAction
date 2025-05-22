#include "Viewport.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"

Viewport::Viewport(DirectXCommand* directXCommand) {
	Initialize(directXCommand);
	Logger::Log("Viewport Initialize\n");
}

Viewport::~Viewport() {
	Logger::Log("Viewport Finalize\n");
}

void Viewport::Initialize(DirectXCommand* directXCommand) {
	SetDirectXCommand(directXCommand);
}

void Viewport::SettingViewport(uint32_t width, uint32_t height) {
	// ビューポートの設定
	D3D12_VIEWPORT viewport{
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<float>(width),
		.Height = static_cast<float>(height),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f
	};
	directXCommand_->GetList()->RSSetViewports(1, &viewport);
}

void Viewport::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}
