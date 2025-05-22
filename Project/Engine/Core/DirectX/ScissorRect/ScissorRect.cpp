#include "ScissorRect.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "WindowApp/WindowApp.h"

ScissorRect::ScissorRect(DirectXCommand* directXCommand) {
	Initialize(directXCommand);
	Logger::Log("ScissorRect Initialize\n");
}

ScissorRect::~ScissorRect() {
	Logger::Log("ScissorRect Finalize\n");
}

void ScissorRect::Initialize(DirectXCommand* directXCommand) {
	SetDirectXCommand(directXCommand);
}

void ScissorRect::SettingScissorRect(uint32_t width, uint32_t height) {
	// シザー矩形の設定
	D3D12_RECT scissorRect{
		.left = 0,
        .top = 0,
        .right = static_cast<LONG>(width),
        .bottom = static_cast<LONG>(height)
	};
	directXCommand_->GetList()->RSSetScissorRects(1, &scissorRect);
}

void ScissorRect::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}
