#include "Fence.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"

#include "Logger/Logger.h"

Fence::Fence(DXGI* dxgi, DirectXCommand* directXCommand) {
	Initialize(dxgi, directXCommand);
	Logger::Log("Fence Initialize\n");
}

Fence::~Fence() {
	Logger::Log("Fence Finalize\n");
}

void Fence::Initialize(DXGI* dxgi, DirectXCommand* directXCommand) {
	// dxgiのインスタンスをセット
	SetDXGI(dxgi);
	// コマンドのインスタンスをセット
	SetCommand(directXCommand);
	// フェンスバリューを0
	fenceValue_ = 0;
	// フェンスの作成
	CreateFence();
}

void Fence::WaitGPU() {
	// Fenceの値を更新し、GPUGPUがここまでたどり着いたときにFenceの値を指定した値に代入するようにSignalを送る
	directXCommand_->GetQueue()->Signal(fence_.Get(), ++fenceValue_);
	// Fenceの値が指定されたSignal値にたどり着いているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceValue_) {
		// FenceのSignalを待つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent);
		// イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Fence::CreateFence() {
	// 初期値0でフェンスを作る
	hr_ = dxgi_->GetDevice()->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr_));
}

void Fence::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void Fence::SetCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}
