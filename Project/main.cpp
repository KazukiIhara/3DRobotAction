
#include <Windows.h>
#include <memory>

#include "Logger/Logger.h"
#include "Onterminate/OnTerminate.h"

#include "Game/Game.h"

// Lib
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#pragma comment(lib,"winmm.lib")

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib, "xinput.lib")
#pragma comment(lib,"xaudio2.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// 例外終了時処理
	std::set_terminate(OnTerminate);

	// ログ出力ファイル作成
	Logger::Initialize();

	// 開始ログ
	Logger::Log("Hello,MAGI ENGINE!\n");

	// ゲーム実行
	std::unique_ptr<MAGISYSTEM> game = std::make_unique<Game>();
	game->Run();

	// 終了ログ
	Logger::Log("Bye,MAGI ENGINE!\n");

	// ログ終了
	Logger::Finalize();

	return 0;
}

