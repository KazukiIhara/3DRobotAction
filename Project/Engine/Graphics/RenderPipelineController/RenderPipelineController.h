#pragma once

#include <vector>
#include <memory>

#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/PostEffectStruct.h"

// シーンカラー用のレンダーテクスチャ
#include "ResourceTextures/RenderTextures/ColorRenderTexture/ColorRenderTexture.h"

// GBuffe用のレンダーテクスチャ
#include "ResourceTextures/RenderTextures/GBuffers/GBufferAlbedoRenderTexture/GBufferAlbedoRenderTexture.h"
#include "ResourceTextures/RenderTextures/GBuffers/GBufferNormalRenderTexture/GBufferNormalRenderTexture.h"
#include "ResourceTextures/RenderTextures/GBuffers/GBufferPositionRenderTexture/GBufferPositionRenderTexture.h"

// シャドウマップ用の深度テクスチャ
#include "ResourceTextures/DepthTextures/ShadowDepthTexture/ShadowDepthTexture.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class DepthStencil;
class Viewport;
class ScissorRect;
class RTVManager;
class SRVUAVManager;
class DefferedRenderringPipelineManager;
class PostEffectPipelineManager;
class ShadowPipelineManager;
class Camera3DManager;
class LightManager;
class SkyBoxDrawer;

/// <summary>
/// 描画管理クラス
/// </summary>
class RenderController {
public:
	RenderController(
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
	);
	~RenderController() = default;

	// シャドウマップ用の深度描画前準備
	void PreShadowRender();

	// シャドウマップ用の深度描画後処理
	void PostShadowRender();

	// シーンを描画するための前準備
	void PreRenderForGBuffers();

	// シーン描画後の処理
	void PostRenderForGBuffers();

	// シーン用のレンダーテクスチャに描画する前の処理
	void PreSceneRender();

	// ライト適用
	void LightingPass();

	// ライト適用後の処理
	void PostSceneRender();

	// ポストエフェクトをpingPong方式でかけていく
	void ApplyPostEffect();

	// 最終描画用のテクスチャに描画
	void RenderToFinalRenderTexture();

	// スワップチェーンに最終描画用のレンダーテクスチャを描画
	void RenderToSwapChain();

	// フレーム終了処理
	void EndFrame();

	// ポストエフェクト追加
	void AddPostEffect(const PostEffectCommand& command);
private:
	// カラーレンダーテクスチャのインデックス切り替え
	void SwitchColorRenderTextureIndex();

	// レンダーテクスチャを描画
	void DrawRenderTextureNoParamater(ID3D12GraphicsCommandList* commandList, const PostEffectType& type);
	void DrawRenderTextureWithParamater(ID3D12GraphicsCommandList* commandList, const PostEffectCommand& command);
	void DrawRenderTextureWithParamaterAndDepth(ID3D12GraphicsCommandList* commandList, const PostEffectCommand& command);
	void DrawRenderTextureWithParamaterDepthAndNormal(ID3D12GraphicsCommandList* commandList, const PostEffectCommand& command);

	// 複数のレンダーターゲットを追加
	void SetRenderTargets(const std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2>& rtvs, D3D12_CPU_DESCRIPTOR_HANDLE dsv);

	// パラメータ用のリソースを作成する
	void CreatePostEffectParamaterResource();

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetDepthStencil(DepthStencil* depthStencil);
	void SetViewport(Viewport* viewport);
	void SetScissorRect(ScissorRect* scissorRect);
	void SetRTVManager(RTVManager* rtvManager);
	void SetSrvUavManager(SRVUAVManager* srvUavManager);
	void SetDefferedRenderringPipelineManager(DefferedRenderringPipelineManager* defferedRenderringPipelineManager);
	void SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager);
	void SetShadowPipelineManager(ShadowPipelineManager* shadowPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);
	void SetLightManager(LightManager* lightManager);
	void SetSkyBoxDrawer(SkyBoxDrawer* skyBoxDrawer);

private:
	// 各インスタンスを受け取るクラス
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	DepthStencil* depthStencil_ = nullptr;
	Viewport* viewport_ = nullptr;
	ScissorRect* scissorRect_ = nullptr;
	RTVManager* rtvManager_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	DefferedRenderringPipelineManager* defferedRenderringPipelineManager_ = nullptr;
	PostEffectPipelineManager* postEffectPipelineManager_ = nullptr;
	ShadowPipelineManager* shadowPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
	LightManager* lightManager_ = nullptr;
	SkyBoxDrawer* skyBoxDrawer_ = nullptr;

private:
	// コマンド最大数
	static const uint32_t kMaxPostEffectNum_ = 64;

	// 現在のレンダーターゲット
	BaseRenderTexture* currentRenderTarget_ = nullptr;
	// これから描画するレンダーテクスチャ
	BaseRenderTexture* currentRenderTexture_ = nullptr;

	// シーンを描画するレンダーテクスチャ
	std::unique_ptr<ColorRenderTexture> sceneRenderTexture_ = nullptr;
	// 最終描画用のレンダーテクスチャ
	std::unique_ptr<ColorRenderTexture> finalRenderTexture_ = nullptr;

	//================================================
	// PostEffect用
	//================================================ 

	// カラーポストエフェクト用のレンダーテクスチャ
	std::unique_ptr<ColorRenderTexture> colorPostEffectRenderTexture_[2] = { nullptr,nullptr };
	// 現在使用中のカラーポストエフェクト用のレンダーテクスチャインデックス
	uint32_t currentColorPostEffectRenderTextureIndex_ = 0;

	// ポストエフェクトのパラメータ用リソース
	ComPtr<ID3D12Resource> postEffectParamResource_[kMaxPostEffectNum_];
	// ポストエフェクトのパラメータ用データ
	PostEffectParamater* postEffectParamData_[kMaxPostEffectNum_];

	// ポストエフェクトをかけるためのコマンド
	std::vector<PostEffectCommand> postEffectCommand_{};
	// 現在のコマンドインデックス
	uint32_t currentCommandIndex_ = 0;


	//================================================
	// GBuffer用
	//================================================

	// アルベド
	std::unique_ptr<GBufferAlbedoRenderTexture> gBufferAlbedoRenderTexture_ = nullptr;
	// 法線
	std::unique_ptr<GBufferNormalRenderTexture> gBufferNormalRenderTexture_ = nullptr;
	// 座標
	std::unique_ptr<GBufferPositionRenderTexture> gBufferPositionRenderTexture_ = nullptr;


	//================================================
	// ShadowMap用
	//================================================

	// 影深度テクスチャ
	std::unique_ptr<ShadowDepthTexture> shadowDepthTexture_ = nullptr;

};
