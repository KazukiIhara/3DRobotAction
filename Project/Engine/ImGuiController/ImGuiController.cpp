#include "ImGuiController.h"

// MyHedder
#include "Logger/Logger.h"
#include "WindowApp/WindowApp.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

ImGuiController::ImGuiController(WindowApp* windowApp, DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvUavManager) {
	Initialize(windowApp, dxgi, command, srvUavManager);
	Logger::Log("ImGuiController Initialize\n");
}

ImGuiController::~ImGuiController() {
	Finalize();
	Logger::Log("ImGuiController Finalize\n");
}

void ImGuiController::Initialize(WindowApp* windowApp, DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvUavManager) {
	// インスタンスのセット
	SetWindowApp(windowApp);
	SetDXGI(dxgi);
	SetDirectXCommand(command);
	SetSrvUavManager(srvUavManager);

	// srvインデックスを割り当て
	uint32_t srvIndex = srvUavManager_->Allocate();

	// ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(windowApp_->GetHwnd());
	ImGui_ImplDX12_Init(dxgi_->GetDevice(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvUavManager_->GetDescriptorHeap(),
		srvUavManager_->GetDescriptorHandleCPU(srvIndex),
		srvUavManager_->GetDescriptorHandleGPU(srvIndex)
	);
}


void ImGuiController::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiController::SetAllCommand() {
	// ImGui内部コマンドの生成
	ImGui::Render();
	// 実際のCommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_->GetList());
}

void ImGuiController::Finalize() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiController::SetWindowApp(WindowApp* windowApp) {
	assert(windowApp);
	windowApp_ = windowApp;
}

void ImGuiController::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void ImGuiController::SetDirectXCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void ImGuiController::SetSrvUavManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}
