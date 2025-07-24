#include "RenderPipelineController.h"

#include <cassert>
#include <format>
#include <array>

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"
#include "ViewManagers/RTVManager/RTVManager.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/DefferedRenderringPipelineManager/DefferedRenderringPipelineManager.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"
#include "LightManager/LightManager.h"
#include "3D/Drawer3D/SkyBoxDrawer/SkyBoxDrawer.h"

#include "Logger/Logger.h"

RenderController::RenderController(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	DepthStencil* depthStencil,
	Viewport* viewport,
	ScissorRect* scissorRect,
	RTVManager* rtvManager,
	SRVUAVManager* srvUavManager,
	DefferedRenderringPipelineManager* defferedRenderringPipelineManager,
	PostEffectPipelineManager* postEffectPipelineManager,
	ShadowPipelineManager* shadowPipelineManager,
	Camera3DManager* camera3DManager,
	LightManager* lightManager,
	SkyBoxDrawer* skyBoxDrawer
) {
	// インスタンスを受け取る
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetDepthStencil(depthStencil);
	SetViewport(viewport);
	SetScissorRect(scissorRect);
	SetRTVManager(rtvManager);
	SetSrvUavManager(srvUavManager);
	SetDefferedRenderringPipelineManager(defferedRenderringPipelineManager);
	SetPostEffectPipelineManager(postEffectPipelineManager);
	SetShadowPipelineManager(shadowPipelineManager);
	SetCamera3DManager(camera3DManager);
	SetLightManager(lightManager);
	SetSkyBoxDrawer(skyBoxDrawer);

	// パラメータ用のリソースを作成
	CreatePostEffectParamaterResource();

	// シーン描画用のレンダーテクスチャ
	sceneRenderTexture_ = std::make_unique<ColorRenderTexture>();
	sceneRenderTexture_->Initialize();

	// 最終描画用のレンダーテクスチャ
	finalRenderTexture_ = std::make_unique<ColorRenderTexture>();
	finalRenderTexture_->Initialize();

	// カラーポストエフェクト用のレンダーテクスチャ
	for (uint32_t i = 0; i < 2; i++) {
		colorPostEffectRenderTexture_[i] = std::make_unique<ColorRenderTexture>();
		colorPostEffectRenderTexture_[i]->Initialize();
	}

	// GBuffr用のレンダーテクスチャ
	// アルベド
	gBufferAlbedoRenderTexture_ = std::make_unique<GBufferAlbedoRenderTexture>();
	gBufferAlbedoRenderTexture_->Initialize();

	// 法線
	gBufferNormalRenderTexture_ = std::make_unique<GBufferNormalRenderTexture>();
	gBufferNormalRenderTexture_->Initialize();

	// Shadow用の深度テクスチャ
	shadowDepthTexture_ = std::make_unique<ShadowDepthTexture>();

	// コマンドの最大数をあらかじめ決めておく
	postEffectCommand_.resize(kMaxPostEffectNum_);
}

void RenderController::PreShadowRender() {
	// まずリソース状態を「書き込み可能」に遷移
	shadowDepthTexture_->TransitionToWrite();

	// シャドウマップをクリア
	shadowDepthTexture_->Clear();

	// シャドウマップ用DSVセット
	shadowDepthTexture_->SetAsRenderTarget();

	// Viewport、Scissorを設定
	viewport_->SettingViewport(ShadowDepthTexture::kShadowMapWidth, ShadowDepthTexture::kShadowMapHeight);
	scissorRect_->SettingScissorRect(ShadowDepthTexture::kShadowMapWidth, ShadowDepthTexture::kShadowMapHeight);
}

void RenderController::PostShadowRender() {
	shadowDepthTexture_->TransitionToRead();
}

void RenderController::PreRenderForGBuffers() {
	// Gバッファ2枚＋深度バッファをセットする
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2> rtvs = {
		rtvManager_->GetDescriptorHandleCPU(gBufferAlbedoRenderTexture_->GetRtvIndex()),
		rtvManager_->GetDescriptorHandleCPU(gBufferNormalRenderTexture_->GetRtvIndex()),
	};

	// RenderTargetとDepthStencilViewをバインド
	SetRenderTargets(rtvs, depthStencil_->GetDepthStencilResorceCPUHandle());

	// 各レンダーターゲットをクリア
	gBufferAlbedoRenderTexture_->ClearRenderTarget();
	gBufferNormalRenderTexture_->ClearRenderTarget();
	depthStencil_->ClearDepthView();

	// ビューポート、シザー設定
	viewport_->SettingViewport();
	scissorRect_->SettingScissorRect();
}

void RenderController::PostRenderForGBuffers() {
	// ライティング前に、GバッファをSRV用に遷移
	gBufferAlbedoRenderTexture_->TransitionToRead();
	gBufferNormalRenderTexture_->TransitionToRead();

	// 深度バッファをSRV用に遷移
	depthStencil_->TransitionToRead();
}

void RenderController::PreSceneRender() {
	// SceneRenderTextureに書き込む
	sceneRenderTexture_->SetAsRenderTarget(depthStencil_->GetDepthStencilResorceCPUHandle());
	sceneRenderTexture_->ClearRenderTarget();

	// ビューポートとシザー設定
	viewport_->SettingViewport();
	scissorRect_->SettingScissorRect();
}

void RenderController::LightingPass() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネチャとPSOを設定
	commandList->SetGraphicsRootSignature(defferedRenderringPipelineManager_->GetRootSignature(DefferedRenderringType::Lighting));
	commandList->SetPipelineState(defferedRenderringPipelineManager_->GetPipelineState(DefferedRenderringType::Lighting));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// カメラを転送
	camera3DManager_->TransferCurrentCamera(0);

	// DirectinalLightを転送
	lightManager_->TransferDirectionalLight(1);

	// DirectionalLightCamera転送
	lightManager_->TransferDirectionalLightCamera(2);

	// カメラの逆行列をセット
	camera3DManager_->TransferCurrentCameraInverse(3);

	// GBufferのSRVをセット（t0, t1, t2）
	commandList->SetGraphicsRootDescriptorTable(4, srvUavManager_->GetDescriptorHandleGPU(gBufferAlbedoRenderTexture_->GetSrvIndex()));
	commandList->SetGraphicsRootDescriptorTable(5, srvUavManager_->GetDescriptorHandleGPU(gBufferNormalRenderTexture_->GetSrvIndex()));
	commandList->SetGraphicsRootDescriptorTable(6, srvUavManager_->GetDescriptorHandleGPU(depthStencil_->GetSrvIndex()));

	// シャドウマップテクスチャのSRVをセット
	commandList->SetGraphicsRootDescriptorTable(7, srvUavManager_->GetDescriptorHandleGPU(shadowDepthTexture_->GetSrvIndex()));

	// 環境マップテクスチャのSRVをセット
	skyBoxDrawer_->TransferSkyBoxTexture(8);

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

void RenderController::PostSceneRender() {
	// シーン描画用のレンダーターゲットを読み取り状態に
	sceneRenderTexture_->TransitionToRead();
	// 現在のテクスチャをシーン描画結果に
	currentRenderTexture_ = sceneRenderTexture_.get();
}

void RenderController::ApplyPostEffect() {
	// 今回積まれているポストエフェクトの数を取得
	const uint32_t currentFramePostEffectNum = currentCommandIndex_;
	if (currentFramePostEffectNum == 0) {
		return; // 何もなければリターン
	}

	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	for (uint32_t i = 0; i < currentFramePostEffectNum; i++) {
		auto& command = postEffectCommand_[i];

		switch (command.postEffectType) {
		case PostEffectType::Copy:
		case PostEffectType::Grayscale:
			DrawRenderTextureNoParamater(commandList, command.postEffectType);
			break;
		case PostEffectType::Vignette:
		case PostEffectType::GaussianX:
		case PostEffectType::GaussianY:
		case PostEffectType::RadialBlur:
		case PostEffectType::LuminanceOutline:
			DrawRenderTextureWithParamater(commandList, command);
			break;
		case PostEffectType::DepthOutline:
			DrawRenderTextureWithParamaterAndDepth(commandList, command);
			break;
		}

	}

	// 最後の結果がcurrentRenderTexture_に入っている
	currentRenderTarget_ = nullptr; // 使い終わったのでnullに
	currentCommandIndex_ = 0; // コマンドバッファをリセット
}

void RenderController::RenderToFinalRenderTexture() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// レンダーターゲットを最終描画用のレンダーテクスチャに指定
	finalRenderTexture_->SetAsRenderTarget();
	finalRenderTexture_->ClearRenderTarget();
	// ビューポートの設定
	viewport_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();

	// 最終レンダーテクスチャに描画
	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(PostEffectType::Copy));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(PostEffectType::Copy, BlendMode::None));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタハンドルを指定
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(currentRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);

	// 最終レンダーテクスチャを読み取り可能状態にする
	finalRenderTexture_->TransitionToRead();
}

void RenderController::RenderToSwapChain() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(PostEffectType::Copy));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(PostEffectType::Copy, BlendMode::None));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタハンドルを指定
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(finalRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

void RenderController::EndFrame() {
	// レンダーテクスチャをnullptrに
	currentRenderTarget_ = nullptr;
	currentRenderTexture_ = nullptr;

	// 次のフレーム用に書き込み可能状態にする
	gBufferAlbedoRenderTexture_->TransitionToWrite();
	gBufferNormalRenderTexture_->TransitionToWrite();

	depthStencil_->TransitionToWrite();

	sceneRenderTexture_->TransitionToWrite();
	finalRenderTexture_->TransitionToWrite();

	for (uint32_t i = 0; i < 2; i++) {
		colorPostEffectRenderTexture_[i]->TransitionToWrite();
	}

}

void RenderController::AddPostEffect(const PostEffectCommand& command) {
	// コマンドを追加
	postEffectCommand_[currentCommandIndex_] = command;
	// インデックスを登録
	postEffectCommand_[currentCommandIndex_].index = currentCommandIndex_;
	// コマンドインデックスをインクリメント
	currentCommandIndex_++;
}

void RenderController::SwitchColorRenderTextureIndex() {
	if (currentColorPostEffectRenderTextureIndex_ == 0) {
		currentColorPostEffectRenderTextureIndex_ = 1;
	} else {
		currentColorPostEffectRenderTextureIndex_ = 0;
	}
}

void RenderController::DrawRenderTextureNoParamater(ID3D12GraphicsCommandList* commandList, const PostEffectType& type) {
	// 現在の書き込み先のレンダーテクスチャを切り替え
	currentRenderTarget_ = colorPostEffectRenderTexture_[currentColorPostEffectRenderTextureIndex_].get();
	// 書き込み可能状態にする
	currentRenderTarget_->TransitionToWrite();
	// 次のポストエフェクト用にレンダーテクスチャを切り替え
	SwitchColorRenderTextureIndex();

	// レンダーターゲットを設定
	currentRenderTarget_->SetAsRenderTarget();
	currentRenderTarget_->ClearRenderTarget();

	// ビューポート、シザー設定
	viewport_->SettingViewport();
	scissorRect_->SettingScissorRect();

	// ポストエフェクトに対応するパイプラインを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(type));
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(type, BlendMode::None));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 入力するテクスチャはひとつ前に描画したレンダーテクスチャ
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(currentRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);

	// 出力結果を次の入力にする
	currentRenderTexture_ = currentRenderTarget_;

	// 描画した対象を読み取り可能状態にする
	currentRenderTexture_->TransitionToRead();
}

void RenderController::DrawRenderTextureWithParamater(ID3D12GraphicsCommandList* commandList, const PostEffectCommand& command) {
	// 現在の書き込み先のレンダーテクスチャを切り替え
	currentRenderTarget_ = colorPostEffectRenderTexture_[currentColorPostEffectRenderTextureIndex_].get();
	// 書き込み可能状態にする
	currentRenderTarget_->TransitionToWrite();
	// 次のポストエフェクト用にレンダーテクスチャを切り替え
	SwitchColorRenderTextureIndex();

	// レンダーターゲットを設定
	currentRenderTarget_->SetAsRenderTarget();
	currentRenderTarget_->ClearRenderTarget();

	// ビューポート、シザー設定
	viewport_->SettingViewport();
	scissorRect_->SettingScissorRect();

	// パラメータを更新
	std::memcpy(postEffectParamData_[command.index], &command.param, sizeof(PostEffectParamater));

	// ポストエフェクトに対応するパイプラインを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(command.postEffectType));
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(command.postEffectType, BlendMode::None));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 入力するテクスチャはひとつ前に描画したレンダーテクスチャ
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(currentRenderTexture_->GetSrvIndex()));

	// パラメータを送信
	commandList->SetGraphicsRootConstantBufferView(1, postEffectParamResource_[command.index]->GetGPUVirtualAddress());

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);

	// 出力結果を次の入力にする
	currentRenderTexture_ = currentRenderTarget_;

	// 描画した対象を読み取り可能状態にする
	currentRenderTexture_->TransitionToRead();

}

void RenderController::DrawRenderTextureWithParamaterAndDepth(ID3D12GraphicsCommandList* commandList, const PostEffectCommand& command) {
	// 現在の書き込み先のレンダーテクスチャを切り替え
	currentRenderTarget_ = colorPostEffectRenderTexture_[currentColorPostEffectRenderTextureIndex_].get();
	// 書き込み可能状態にする
	currentRenderTarget_->TransitionToWrite();
	// 次のポストエフェクト用にレンダーテクスチャを切り替え
	SwitchColorRenderTextureIndex();

	// レンダーターゲットを設定
	currentRenderTarget_->SetAsRenderTarget();
	currentRenderTarget_->ClearRenderTarget();

	// ビューポート、シザー設定
	viewport_->SettingViewport();
	scissorRect_->SettingScissorRect();

	// パラメータを更新
	std::memcpy(postEffectParamData_[command.index], &command.param, sizeof(PostEffectParamater));

	// ポストエフェクトに対応するパイプラインを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(command.postEffectType));
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(command.postEffectType, BlendMode::None));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 入力するテクスチャはひとつ前に描画したレンダーテクスチャ
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(currentRenderTexture_->GetSrvIndex()));

	// パラメータを送信
	commandList->SetGraphicsRootConstantBufferView(1, postEffectParamResource_[command.index]->GetGPUVirtualAddress());

	// 深度を送信
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(depthStencil_->GetSrvIndex()));

	// カメラの逆行列を送る
	camera3DManager_->TransferCurrentCameraInverse(3);

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);

	// 出力結果を次の入力にする
	currentRenderTexture_ = currentRenderTarget_;

	// 描画した対象を読み取り可能状態にする
	currentRenderTexture_->TransitionToRead();
}

void RenderController::SetRenderTargets(const std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2>& rtvs, D3D12_CPU_DESCRIPTOR_HANDLE dsv) {
	directXCommand_->GetList()->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), FALSE, &dsv);
}

void RenderController::CreatePostEffectParamaterResource() {
	for (uint32_t i = 0; i < kMaxPostEffectNum_; i++) {
		postEffectParamResource_[i] = dxgi_->CreateBufferResource(sizeof(PostEffectParamater));
		HRESULT hr = postEffectParamResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&postEffectParamData_[i]));
		assert(SUCCEEDED(hr));
	}
}

void RenderController::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void RenderController::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void RenderController::SetDepthStencil(DepthStencil* depthStencil) {
	assert(depthStencil);
	depthStencil_ = depthStencil;
}

void RenderController::SetViewport(Viewport* viewport) {
	assert(viewport);
	viewport_ = viewport;
}

void RenderController::SetScissorRect(ScissorRect* scissorRect) {
	assert(scissorRect);
	scissorRect_ = scissorRect;
}

void RenderController::SetRTVManager(RTVManager* rtvManager) {
	assert(rtvManager);
	rtvManager_ = rtvManager;
}

void RenderController::SetSrvUavManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void RenderController::SetDefferedRenderringPipelineManager(DefferedRenderringPipelineManager* defferedRenderringPipelineManager) {
	assert(defferedRenderringPipelineManager);
	defferedRenderringPipelineManager_ = defferedRenderringPipelineManager;
}

void RenderController::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager) {
	assert(postEffectPipelineManager);
	postEffectPipelineManager_ = postEffectPipelineManager;
}

void RenderController::SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager) {
	assert(shadowPipelineManager);
	shadowPipelineManager_ = shadowPipelineManager;
}

void RenderController::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void RenderController::SetLightManager(LightManager* lightManager) {
	assert(lightManager);
	lightManager_ = lightManager;
}

void RenderController::SetSkyBoxDrawer(SkyBoxDrawer* skyBoxDrawer) {
	assert(skyBoxDrawer);
	skyBoxDrawer_ = skyBoxDrawer;
}
