#include "MAGI.h"

#include "Logger/Logger.h"

#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

// Staticメンバ変数の初期化
#if defined(DEBUG) || defined(DEVELOP)
std::unique_ptr<D3DResourceLeakChecker> MAGISYSTEM::leakCheck_ = nullptr;
#endif // _DEBUG

// 
// BaseSystems
// 
std::unique_ptr<WindowApp> MAGISYSTEM::windowApp_ = nullptr;
std::unique_ptr<DeltaTimer> MAGISYSTEM::deltaTimer_ = nullptr;
std::unique_ptr<MAGIDirectInput> MAGISYSTEM::directInput_ = nullptr;
std::unique_ptr<MAGIXInput> MAGISYSTEM::xInput_ = nullptr;

// 
// DirectXBaseSystems
// 
std::unique_ptr<DXGI> MAGISYSTEM::dxgi_ = nullptr;
std::unique_ptr<DirectXCommand> MAGISYSTEM::directXCommand_ = nullptr;
std::unique_ptr<Fence> MAGISYSTEM::fence_ = nullptr;
std::unique_ptr<ShaderCompiler> MAGISYSTEM::shaderCompiler_ = nullptr;

// 
// ViewManagers
// 
std::unique_ptr<RTVManager> MAGISYSTEM::rtvManager_ = nullptr;
std::unique_ptr<DSVManager> MAGISYSTEM::dsvManager_ = nullptr;
std::unique_ptr<SRVUAVManager> MAGISYSTEM::srvuavManager_ = nullptr;

// 
// DirectXRenderSystems
// 
std::unique_ptr<SwapChain> MAGISYSTEM::swapChain_ = nullptr;
std::unique_ptr<DepthStencil> MAGISYSTEM::depthStencil_ = nullptr;
std::unique_ptr<Viewport> MAGISYSTEM::viewport_ = nullptr;
std::unique_ptr<ScissorRect> MAGISYSTEM::scissorRect_ = nullptr;

//
// PipelineManager
//
std::unique_ptr<GraphicsPipelineManager> MAGISYSTEM::graphicsPipelineManager_ = nullptr;
std::unique_ptr<ComputePipelineManager> MAGISYSTEM::computePipelineManager_ = nullptr;
std::unique_ptr<DefferedRenderringPipelineManager> MAGISYSTEM::defferedRenderringPipelineManager_ = nullptr;
std::unique_ptr<PostEffectPipelineManager> MAGISYSTEM::postEffectPipelineManager_ = nullptr;
std::unique_ptr<ShadowPipelineManager> MAGISYSTEM::shadowPipelineManager_ = nullptr;

// 
// AssetContainer
// 
std::unique_ptr<TextureDataContainer> MAGISYSTEM::textureDataCantainer_ = nullptr;
std::unique_ptr<PrimitiveShapeDataContainer> MAGISYSTEM::primitiveDataContainer_ = nullptr;
std::unique_ptr<ModelDataContainer> MAGISYSTEM::modelDataContainer_ = nullptr;
std::unique_ptr<AnimationDataContainer> MAGISYSTEM::animationDataContainer_ = nullptr;
std::unique_ptr<SoundDataContainer> MAGISYSTEM::soundDataContainer_ = nullptr;
std::unique_ptr<SceneDataContainer> MAGISYSTEM::sceneDataContainer_ = nullptr;

// 
// ComponentManager
// 
std::unique_ptr<TransformManager> MAGISYSTEM::transformManager_ = nullptr;
std::unique_ptr<Renderer3DManager> MAGISYSTEM::renderer3DManager_ = nullptr;

//
// ObjectManager
//
std::unique_ptr<GameObject3DManager> MAGISYSTEM::gameObject3DManager_ = nullptr;
std::unique_ptr<Camera2DManager> MAGISYSTEM::camera2DManager_ = nullptr;
std::unique_ptr<Camera3DManager> MAGISYSTEM::camera3DManager_ = nullptr;
std::unique_ptr<Emitter3DManager> MAGISYSTEM::emitter3DManager_ = nullptr;
std::unique_ptr<ParticleGroup3DManager> MAGISYSTEM::particleGroup3DManager_ = nullptr;
std::unique_ptr<LightManager> MAGISYSTEM::lightManager_ = nullptr;

// 
// Drawer
// 
std::unique_ptr<SpriteDrawer> MAGISYSTEM::spriteDrawer_ = nullptr;

std::unique_ptr<LineDrawer3D> MAGISYSTEM::lineDrawer3D_ = nullptr;
std::unique_ptr<TriangleDrawer3D> MAGISYSTEM::triangleDrawer3D_ = nullptr;
std::unique_ptr<PlaneDrawer3D> MAGISYSTEM::planeDrawer3D_ = nullptr;
std::unique_ptr<BoxDrawer3D> MAGISYSTEM::boxDrawer3D_ = nullptr;
std::unique_ptr<SphereDrawer3D> MAGISYSTEM::sphereDrawer3D_ = nullptr;
std::unique_ptr<RingDrawer3D> MAGISYSTEM::ringDrawer3D_ = nullptr;
std::unique_ptr<CylinderDrawer3D> MAGISYSTEM::cylinderDrawer3D_ = nullptr;

std::unique_ptr<ModelDrawerManager> MAGISYSTEM::modelDrawerManager_ = nullptr;

std::unique_ptr<SkyBoxDrawer> MAGISYSTEM::skyBoxDrawer_ = nullptr;

//
// EffectManager
//
std::unique_ptr<PlaneEffectManager> MAGISYSTEM::planeEffectManager_ = nullptr;

//
// AppSystem
//
std::unique_ptr<RenderController> MAGISYSTEM::renderController_ = nullptr;

// 
// GameManager
// 
std::unique_ptr<SceneManager<GameData>> MAGISYSTEM::sceneManager_ = nullptr;

//
// Data入出力クラス
//
std::unique_ptr<GrobalDataManager> MAGISYSTEM::grobalDataManager_ = nullptr;
std::unique_ptr<SceneDataImporter> MAGISYSTEM::sceneDataImporter_ = nullptr;

//
// UIクラス
//
std::unique_ptr<ImGuiController> MAGISYSTEM::imguiController_ = nullptr;
std::unique_ptr<GUI> MAGISYSTEM::gui_ = nullptr;

void MAGISYSTEM::Initialize() {
	// 開始ログ
	Logger::Log("MAGISYSTEM Start\n");

#if defined(DEBUG) || defined(DEVELOP)
	// リークチェッカ
	leakCheck_ = std::make_unique<D3DResourceLeakChecker>();
#endif // _DEBUG

	// WindowApp
	windowApp_ = std::make_unique<WindowApp>();
	// DeltaTimer
	deltaTimer_ = std::make_unique<DeltaTimer>();
	// DirectInput
	directInput_ = std::make_unique<MAGIDirectInput>(windowApp_.get());
	// XInput
	xInput_ = std::make_unique<MAGIXInput>(deltaTimer_.get());

	// DXGI
	dxgi_ = std::make_unique<DXGI>();
	// デバイスがDX12Ultimateに対応しているかどうかを取得
	isSupportDX12Ultimate_ = dxgi_->Initialize();

	// DirectXCommand
	directXCommand_ = std::make_unique<DirectXCommand>(dxgi_.get(), isSupportDX12Ultimate_);
	// Fence
	fence_ = std::make_unique<Fence>(dxgi_.get(), directXCommand_.get());
	// ShaderCompiler
	shaderCompiler_ = std::make_unique<ShaderCompiler>();

	// RTVManager
	rtvManager_ = std::make_unique<RTVManager>(dxgi_.get());
	// DSVManager
	dsvManager_ = std::make_unique<DSVManager>(dxgi_.get());
	// SRVUAVmanager
	srvuavManager_ = std::make_unique<SRVUAVManager>(dxgi_.get());

	// Viewport
	viewport_ = std::make_unique<Viewport>(directXCommand_.get());
	// Scissor
	scissorRect_ = std::make_unique<ScissorRect>(directXCommand_.get());
	// DepthStencil
	depthStencil_ = std::make_unique<DepthStencil>(dxgi_.get(), directXCommand_.get(), dsvManager_.get(), srvuavManager_.get());
	// SwapChain
	swapChain_ = std::make_unique<SwapChain>(windowApp_.get(), dxgi_.get(), viewport_.get(), scissorRect_.get(), directXCommand_.get(), rtvManager_.get());

	// TextureDataContainer
	textureDataCantainer_ = std::make_unique<TextureDataContainer>(dxgi_.get(), directXCommand_.get(), fence_.get(), srvuavManager_.get());
	// PrimitiveDataContainer
	primitiveDataContainer_ = std::make_unique<PrimitiveShapeDataContainer>();
	// ModelDataContainer
	modelDataContainer_ = std::make_unique<ModelDataContainer>(textureDataCantainer_.get());
	// AnimationDataContainer
	animationDataContainer_ = std::make_unique<AnimationDataContainer>();
	// SoundDataContainer
	soundDataContainer_ = std::make_unique<SoundDataContainer>();
	// SceneDataContainer
	sceneDataContainer_ = std::make_unique<SceneDataContainer>();

	// TransformManager
	transformManager_ = std::make_unique<TransformManager>();
	// Renderr3DManager
	renderer3DManager_ = std::make_unique<Renderer3DManager>();


	// GameObject3DManager
	gameObject3DManager_ = std::make_unique<GameObject3DManager>();


	// Camera2DManager
	camera2DManager_ = std::make_unique<Camera2DManager>();
	// Camera3DManager
	camera3DManager_ = std::make_unique<Camera3DManager>();
	// Emitter3DManager
	emitter3DManager_ = std::make_unique<Emitter3DManager>();
	// ParticleGroup3DManager
	particleGroup3DManager_ = std::make_unique<ParticleGroup3DManager>();
	// LightManager
	lightManager_ = std::make_unique<LightManager>(dxgi_.get(), directXCommand_.get());


	// GraphicsPipelineManager
	graphicsPipelineManager_ = std::make_unique<GraphicsPipelineManager>(dxgi_.get(), shaderCompiler_.get());
	// ComputePipelineManager
	computePipelineManager_ = std::make_unique<ComputePipelineManager>(dxgi_.get(), shaderCompiler_.get());
	// DefferedRenderringPipelineManager
	defferedRenderringPipelineManager_ = std::make_unique<DefferedRenderringPipelineManager>(dxgi_.get(), shaderCompiler_.get());
	// PostEffectPipelineManager
	postEffectPipelineManager_ = std::make_unique<PostEffectPipelineManager>(dxgi_.get(), shaderCompiler_.get());
	// ShadowPipelineManager
	shadowPipelineManager_ = std::make_unique<ShadowPipelineManager>(dxgi_.get(), shaderCompiler_.get());


	// SpriteDrawer
	spriteDrawer_ = std::make_unique<SpriteDrawer>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera2DManager_.get());
	// LineDrawer3D
	lineDrawer3D_ = std::make_unique<LineDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera3DManager_.get());
	// TriangleDrawer3D
	triangleDrawer3D_ = std::make_unique<TriangleDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera3DManager_.get());
	// PlaneDrawer3D
	planeDrawer3D_ = std::make_unique<PlaneDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera3DManager_.get());
	// BoxDrawer3D
	boxDrawer3D_ = std::make_unique<BoxDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), shadowPipelineManager_.get(), camera3DManager_.get(), lightManager_.get());
	// SphereDrawer3D
	sphereDrawer3D_ = std::make_unique<SphereDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), shadowPipelineManager_.get(), camera3DManager_.get(), lightManager_.get());
	// RingDrawer3D
	ringDrawer3D_ = std::make_unique<RingDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera3DManager_.get());
	// CylinderDrawer3D
	cylinderDrawer3D_ = std::make_unique<CylinderDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), shadowPipelineManager_.get(), camera3DManager_.get(), lightManager_.get());

	// ModelDrawerManager
	modelDrawerManager_ = std::make_unique<ModelDrawerManager>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), shadowPipelineManager_.get(), camera3DManager_.get());

	// SkyBoxDrawer
	skyBoxDrawer_ = std::make_unique<SkyBoxDrawer>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera3DManager_.get());

	// PlaneEffectManager
	planeEffectManager_ = std::make_unique<PlaneEffectManager>(deltaTimer_.get(), transformManager_.get(), planeDrawer3D_.get());

	// RenderPipelineController
	renderController_ = std::make_unique<RenderController>(
		dxgi_.get(), directXCommand_.get(), depthStencil_.get(), viewport_.get(), scissorRect_.get(),
		rtvManager_.get(), srvuavManager_.get(), defferedRenderringPipelineManager_.get(), postEffectPipelineManager_.get(), shadowPipelineManager_.get(),
		camera3DManager_.get(), lightManager_.get(),
		skyBoxDrawer_.get()
	);

	// SceneManager
	sceneManager_ = std::make_unique<SceneManager<GameData>>();

	// GrobalDataManager
	grobalDataManager_ = std::make_unique<GrobalDataManager>();
	// SceneDataImporter
	sceneDataImporter_ = std::make_unique<SceneDataImporter>(sceneDataContainer_.get(), gameObject3DManager_.get(), camera3DManager_.get(), renderer3DManager_.get(), transformManager_.get());

	// ImGuiController
	imguiController_ = std::make_unique<ImGuiController>(windowApp_.get(), dxgi_.get(), directXCommand_.get(), srvuavManager_.get());
	// GUI
	gui_ = std::make_unique<GUI>(deltaTimer_.get(), srvuavManager_.get());

	// 初期化完了ログ
	Logger::Log("MAGISYSTEM Initialize\n");
}

void MAGISYSTEM::Finalize() {

	// GUI
	if (gui_) {
		gui_.reset();
	}

	// ImGuiController
	if (imguiController_) {
		imguiController_.reset();
	}

	// SceneDataImporter
	if (sceneDataImporter_) {
		sceneDataImporter_.reset();
	}

	// GrobalDataManager
	if (grobalDataManager_) {
		grobalDataManager_.reset();
	}

	// SceneManager
	if (sceneManager_) {
		sceneManager_.reset();
	}

	// RenderController
	if (renderController_) {
		renderController_.reset();
	}

	// PlaneEffectManager
	if (planeEffectManager_) {
		planeEffectManager_.reset();
	}

	// SkyBoxDrawer
	if (skyBoxDrawer_) {
		skyBoxDrawer_.reset();
	}

	// ModelDrawerManager
	if (modelDrawerManager_) {
		modelDrawerManager_.reset();
	}

	// CylinderDrawer3D
	if (cylinderDrawer3D_) {
		cylinderDrawer3D_.reset();
	}

	// RingDrawer3D
	if (ringDrawer3D_) {
		ringDrawer3D_.reset();
	}

	// SphereDrawer3D
	if (sphereDrawer3D_) {
		sphereDrawer3D_.reset();
	}

	// BoxDrawer3D
	if (boxDrawer3D_) {
		boxDrawer3D_.reset();
	}

	// PlaneDrawer3D
	if (planeDrawer3D_) {
		planeDrawer3D_.reset();
	}

	// TriangleDrawer3D
	if (triangleDrawer3D_) {
		triangleDrawer3D_.reset();
	}

	// LineDrawer3D
	if (lineDrawer3D_) {
		lineDrawer3D_.reset();
	}

	// SpriteDrawer
	if (spriteDrawer_) {
		spriteDrawer_.reset();
	}

	// ShadowPipelineManager
	if (shadowPipelineManager_) {
		shadowPipelineManager_.reset();
	}

	// PostEffectPipelineManager
	if (postEffectPipelineManager_) {
		postEffectPipelineManager_.reset();
	}

	// DefferedRenderringPipelineManager
	if (defferedRenderringPipelineManager_) {
		defferedRenderringPipelineManager_.reset();
	}

	// CompuetPipelineManager
	if (computePipelineManager_) {
		computePipelineManager_.reset();
	}

	// GraphicsPipelineManager
	if (graphicsPipelineManager_) {
		graphicsPipelineManager_.reset();
	}

	// LightManager
	if (lightManager_) {
		lightManager_.reset();
	}

	// ParticleGroup3DManager
	if (particleGroup3DManager_) {
		particleGroup3DManager_.reset();
	}

	// Emitter3DManager
	if (emitter3DManager_) {
		emitter3DManager_.reset();
	}

	// Camera3DManager
	if (camera3DManager_) {
		camera3DManager_.reset();
	}

	// Camera2DManager
	if (camera2DManager_) {
		camera2DManager_.reset();
	}

	// GameObject3DManager
	if (gameObject3DManager_) {
		gameObject3DManager_.reset();
	}

	// TransformManager
	if (transformManager_) {
		transformManager_.reset();
	}

	// Renderer3DManager
	if (renderer3DManager_) {
		renderer3DManager_.reset();
	}

	// SceneDataContainer
	if (sceneDataContainer_) {
		sceneDataContainer_.reset();
	}

	// SoundDataContainer
	if (soundDataContainer_) {
		soundDataContainer_.reset();
	}

	// AnimationDataContainer
	if (animationDataContainer_) {
		animationDataContainer_.reset();
	}

	// ModelDataContainer
	if (modelDataContainer_) {
		modelDataContainer_.reset();
	}

	// PrimitiveDataContainer
	if (primitiveDataContainer_) {
		primitiveDataContainer_.reset();
	}

	// TextureDataContainer
	if (textureDataCantainer_) {
		textureDataCantainer_.reset();
	}

	// SwapChain
	if (swapChain_) {
		swapChain_.reset();
	}

	// DepthStencil
	if (depthStencil_) {
		depthStencil_.reset();
	}

	// Scissor
	if (scissorRect_) {
		scissorRect_.reset();
	}

	// Viewport
	if (viewport_) {
		viewport_.reset();
	}

	// SRVUAVManager
	if (srvuavManager_) {
		srvuavManager_.reset();
	}

	// DSVManager
	if (dsvManager_) {
		dsvManager_.reset();
	}

	// RTVManager
	if (rtvManager_) {
		rtvManager_.reset();
	}

	// ShaderCompiler
	if (shaderCompiler_) {
		shaderCompiler_.reset();
	}

	// Fence
	if (fence_) {
		fence_.reset();
	}

	// DirectXCommand
	if (directXCommand_) {
		directXCommand_.reset();
	}

	// DXGI
	if (dxgi_) {
		dxgi_.reset();
	}

	// XInput
	if (xInput_) {
		xInput_.reset();
	}

	// DirectInput
	if (directInput_) {
		directInput_.reset();
	}

	// DeltaTimer
	if (deltaTimer_) {
		deltaTimer_.reset();
	}

	// WindowApp
	if (windowApp_) {
		windowApp_.reset();
	}

	// 終了ログ
	Logger::Log("MAGISYSTEM Finalize\n");

	// ログ終了
	Logger::Finalize();
}

void MAGISYSTEM::Update() {
	// デルタタイムクラスを更新
	deltaTimer_->Update();

	// ウィンドウにメッセージが来ていたら最優先で処理
	if (windowApp_->Update()) {
		endRequest_ = true;
	}

	// 入力の更新
	directInput_->Update();
	xInput_->Update();

	// F11キーでフルスクリーンの切り替え処理
	if (directInput_->TriggerKey(DIK_F11)) {
		windowApp_->ToggleFullScreen();
	}

	// escキーが入力されたらアプリケーションを終了
	if (directInput_->PushKey(DIK_ESCAPE)) {
		endRequest_ = true;
	}

	// デバッグカメラモード切替
	if (directInput_->TriggerKey(DIK_P)) {
		camera3DManager_->GetIsDebugCamera() = !camera3DManager_->GetIsDebugCamera();
	}

	// エンジンUI描画切り替え
	if (directInput_->TriggerKey(DIK_O)) {
		gui_->GetIsShowMainUI() = !gui_->GetIsShowMainUI();
	}

	// ImGui開始処理
	imguiController_->BeginFrame();

	// デバッグ表示(FPS、DELTATIME)
	gui_->ShowDebugUI();

	// シーンの更新処理
	sceneManager_->Update();

	// エフェクトマネージャの更新

	// 板ポリ
	planeEffectManager_->Update();

	// トランスフォームコンポーネントの更新
	transformManager_->Update();

	// 2Dカメラマネージャの更新処理
	camera2DManager_->Update();

	// 3Dカメラマネージャの更新処理
	camera3DManager_->Update();


	// 3Dエミッターマネージャの更新処理
	emitter3DManager_->Update();

	// 3Dパーティクルグループマネージャの更新処理
	particleGroup3DManager_->Update();

	// ライトマネージャ(新)の更新
	lightManager_->Update();


	// グローバルデータ
	grobalDataManager_->Update();

}

void MAGISYSTEM::Draw() {

	// 
	// DirectX描画前処理
	// 

	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// SRVUAVのディスクリプタヒープを設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvuavManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// 
	// 描画処理
	// 

	//
	// シーンの描画処理
	//
	sceneManager_->Draw();

	// 3D描画オブジェクトマネージャー
	renderer3DManager_->Draw();

	// 3Dエフェクト描画

	// 板ポリ
	planeEffectManager_->Draw();

	//==============================================
	// 描画クラスの更新
	//==============================================

	// スプライト描画クラスの更新
	spriteDrawer_->Update();

	// 3Dライン描画クラスの更新
	lineDrawer3D_->Update();
	// 3D三角形描画クラスの更新
	triangleDrawer3D_->Update();
	// 3D板ポリ描画クラスの更新
	planeDrawer3D_->Update();
	// 3Dボックス描画クラスの更新
	boxDrawer3D_->Update();
	// 3D球体描画クラスの更新
	sphereDrawer3D_->Update();
	// 3Dリング描画クラスの更新
	ringDrawer3D_->Update();
	// 3Dシリンダー描画クラスの更新
	cylinderDrawer3D_->Update();

	// モデル描画クラスマネージャの更新
	modelDrawerManager_->UpdateAll();

	// 背景ボックス描画クラスの更新
	skyBoxDrawer_->Update();


	//==============================================
	// ShadowMap用のDepthのみの描画
	//==============================================

	// シャドウマップ用の描画をするための前準備
	renderController_->PreShadowRender();

	// シャドウ用にオブジェクトの描画
	boxDrawer3D_->DrawShadow(BlendMode::None);
	sphereDrawer3D_->DrawShadow(BlendMode::None);
	cylinderDrawer3D_->DrawShadow(BlendMode::None);
	modelDrawerManager_->DrawShadowAll(BlendMode::None);

	// シャドウマップ用の描画後処理
	renderController_->PostShadowRender();


	//==============================================
	// GBufferにシーンを描画
	//==============================================

	// シーン描画用のレンダーテクスチャを準備
	renderController_->PreRenderForGBuffers();

	// オブジェクトの描画(透過なし)
	BlendMode noneMode = BlendMode::None;

	triangleDrawer3D_->Draw(noneMode);
	planeDrawer3D_->Draw(noneMode);
	boxDrawer3D_->Draw(noneMode);
	sphereDrawer3D_->Draw(noneMode);
	ringDrawer3D_->Draw(noneMode);
	cylinderDrawer3D_->Draw(noneMode);

	modelDrawerManager_->DrawAll(noneMode);

	// シーンの描画後処理
	renderController_->PostRenderForGBuffers();


	//==============================================
	// シーン用のレンダーテクスチャに描画
	//==============================================

	// シーン用のレンダーテクスチャに描画する前の処理
	renderController_->PreSceneRender();

	// ライトの適用
	renderController_->LightingPass();

	// 背景ボックスを描画
	skyBoxDrawer_->Draw();

	// ポストエフェクトの影響を受ける背景スプライトを描画
	for (uint32_t i = static_cast<uint32_t>(BlendMode::None); i < kBlendModeNum; ++i) {
		BlendMode mode = static_cast<BlendMode>(i);
		spriteDrawer_->DrawBack(mode);
	}

	// LineDrawer3Dの描画処理
	lineDrawer3D_->Draw();

	// BlendModeごとに描画(透過ありを順番に描画)
	for (uint32_t i = static_cast<uint32_t>(BlendMode::Normal); i < kBlendModeNum; ++i) {
		BlendMode mode = static_cast<BlendMode>(i);
		modelDrawerManager_->DrawAll(mode);
		triangleDrawer3D_->Draw(mode);
		planeDrawer3D_->Draw(mode);
		boxDrawer3D_->Draw(mode);
		sphereDrawer3D_->Draw(mode);
		ringDrawer3D_->Draw(mode);
		cylinderDrawer3D_->Draw(mode);
	}

	// パーティクルの描画処理
	particleGroup3DManager_->Draw();

	// ポストエフェクトの影響を受けるスプライトを描画
	for (uint32_t i = static_cast<uint32_t>(BlendMode::None); i < kBlendModeNum; ++i) {
		BlendMode mode = static_cast<BlendMode>(i);
		spriteDrawer_->DrawFront(mode);
	}

	// シーン用のレンダーテクスチャ描画後処理
	renderController_->PostSceneRender();


	//==============================================
	// ポストプロセス
	//==============================================

	// ポストエフェクトをかける処理
	renderController_->ApplyPostEffect();


	//==============================================
	// SwapChainに投げる前の最終描画
	//==============================================

	// スワップチェーン前最終描画
	renderController_->RenderToFinalRenderTexture();


	//==============================================
	// SwapChainに描画
	//==============================================

	// スワップチェーン描画前処理
	swapChain_->PreRender();

	// スワップチェーンに最終描画用レンダーテクスチャを描画
	renderController_->RenderToSwapChain();

	// ImGui内部コマンド生成
	imguiController_->SetAllCommand();


	//==============================================
	// 描画
	//==============================================

	// スワップチェーンをプレゼント状態に遷移	
	swapChain_->TransitionToPresent();


	//==============================================
	// 次のフレームのための後処理
	//==============================================

	// レンダーコントローラのフレーム終了処理
	renderController_->EndFrame();


	//==============================================
	// コマンド発行
	//==============================================

	// コマンドを閉じて実行
	directXCommand_->KickCommand();
	// GPUとOSに画面の交換を行うように通知
	swapChain_->Present();
	// Fenceによる待機
	fence_->WaitGPU();
	// 次のフレーム用にコマンドをリセット
	directXCommand_->ResetCommand();
}

void MAGISYSTEM::DeleteGarbages() {

	//
	// 削除順に気を付けて実装する(基本外側から消していくイメージ) 
	//

	gameObject3DManager_->DeleteGarbage();

	renderer3DManager_->DeleteGarbage();

	transformManager_->DeleteGarbage();

}

void MAGISYSTEM::Run() {
	// 初期化
	Initialize();
	// メインループ
	while (true) {
		// 更新
		Update();

		// 終了リクエストがあったらループを抜ける;
		if (IsEndRequest()) {
			break;
		}

		// 描画
		Draw();

		// 使用済みオブジェクトを削除
		DeleteGarbages();

	}

	// 終了
	Finalize();
}

bool MAGISYSTEM::IsEndRequest() const {
	return endRequest_;
}

HWND MAGISYSTEM::GetWindowHandle() {
	return windowApp_->GetHwnd();
}

float MAGISYSTEM::GetDeltaTime() {
	return deltaTimer_->GetDeltaTime();
}

bool MAGISYSTEM::PushKey(BYTE keyNumber) {
	return directInput_->PushKey(keyNumber);
}

bool MAGISYSTEM::TriggerKey(BYTE keyNumber) {
	return directInput_->TriggerKey(keyNumber);
}

bool MAGISYSTEM::HoldKey(BYTE keyNumber) {
	return directInput_->HoldKey(keyNumber);
}

bool MAGISYSTEM::ReleaseKey(BYTE keyNumber) {
	return directInput_->ReleaseKey(keyNumber);
}

bool MAGISYSTEM::PushMouseButton(MouseButton mouseButton) {
	return directInput_->PushMouseButton(mouseButton);
}

bool MAGISYSTEM::TriggerMouseButton(MouseButton mouseButton) {
	return directInput_->TriggerMouseButton(mouseButton);
}

bool MAGISYSTEM::HoldMouseButton(MouseButton mouseButton) {
	return directInput_->HoldMouseButton(mouseButton);
}

bool MAGISYSTEM::ReleaseMouseButton(MouseButton mouseButton) {
	return directInput_->ReleaseMouseButton(mouseButton);
}

LONG MAGISYSTEM::GetMouseMoveDeltaX() {
	return directInput_->GetMouseMoveDeltaX();
}

LONG MAGISYSTEM::GetMouseMoveDeltaY() {
	return directInput_->GetMouseMoveDeltaY();
}

int64_t MAGISYSTEM::GetMouseWheelDelta() {
	return directInput_->GetMouseWheelDelta();
}

bool MAGISYSTEM::IsPadConnected(int controllerID) {
	return xInput_->IsConnected(controllerID);
}

bool MAGISYSTEM::PushButton(int controllerID, int buttonNumber) {
	return xInput_->PushButton(controllerID, buttonNumber);
}

bool MAGISYSTEM::TriggerButton(int controllerID, int buttonNumber) {
	return xInput_->TriggerButton(controllerID, buttonNumber);
}

bool MAGISYSTEM::HoldButton(int controllerID, int buttonNumber) {
	return xInput_->HoldButton(controllerID, buttonNumber);
}

bool MAGISYSTEM::ReleaseButton(int controllerID, int buttonNumber) {
	return xInput_->ReleaseButton(controllerID, buttonNumber);
}

Vector2 MAGISYSTEM::GetLeftStick(int controllerID) {
	return xInput_->GetLeftStick(controllerID);
}

Vector2 MAGISYSTEM::GetRightStick(int controllerID) {
	return xInput_->GetRightStick(controllerID);
}

float MAGISYSTEM::GetLeftTrigger(int controllerID) {
	return xInput_->GetLeftTrigger(controllerID);
}

float MAGISYSTEM::GetRightTrigger(int controllerID) {
	return xInput_->GetRightTrigger(controllerID);
}

bool MAGISYSTEM::IsPadUp(int controllerID) {
	return xInput_->IsPadUp(controllerID);
}

bool MAGISYSTEM::IsPadRight(int controllerID) {
	return xInput_->IsPadRight(controllerID);
}

bool MAGISYSTEM::IsPadDown(int controllerID) {
	return xInput_->IsPadDown(controllerID);
}

bool MAGISYSTEM::IsPadLeft(int controllerID) {
	return xInput_->IsPadLeft(controllerID);
}

void MAGISYSTEM::SetDeadZone(int deadZone) {
	xInput_->SetDeadZone(deadZone);
}

int MAGISYSTEM::GetDeadZone() {
	return xInput_->GetDeadZone();
}

void MAGISYSTEM::StartPadVibration(int controllerID, float duration, float leftPower, float rightPower) {
	xInput_->StartVibration(controllerID, duration, leftPower, rightPower);
}

void MAGISYSTEM::StopPadVibration(int controllerID) {
	xInput_->StopVibration(controllerID);
}

ID3D12Device* MAGISYSTEM::GetDirectXDevice() {
	return dxgi_->GetDevice();
}

ComPtr<ID3D12Resource> MAGISYSTEM::CreateBufferResource(size_t sizeInBytes, bool isUav) {
	return dxgi_->CreateBufferResource(sizeInBytes, isUav);
}

ComPtr<ID3D12Resource> MAGISYSTEM::CreateDepthStencilTextureResource(int32_t width, int32_t height, DXGI_FORMAT format) {
	return dxgi_->CreateDepthStencilTextureResource(width, height, format);
}

ID3D12GraphicsCommandList* MAGISYSTEM::GetDirectXCommandList() {
	return directXCommand_->GetList();
}

ID3D12GraphicsCommandList6* MAGISYSTEM::GetDirectXCommandList6() {
	return directXCommand_->GetList6();
}

void MAGISYSTEM::KickCommand() {
	directXCommand_->KickCommand();
}

void MAGISYSTEM::ResetCommand() {
	directXCommand_->ResetCommand();
}

void MAGISYSTEM::WaitGPU() {
	fence_->WaitGPU();
}

ID3D12DescriptorHeap* MAGISYSTEM::GetRTVDescriptorHeap() {
	return rtvManager_->GetDescriptorHeap();
}

D3D12_CPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetRTVDescriptorHandleCPU(uint32_t index) {
	return rtvManager_->GetDescriptorHandleCPU(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetRTVDescriptorHandleGPU(uint32_t index) {
	return rtvManager_->GetDescriptorHandleGPU(index);
}

uint32_t MAGISYSTEM::RTVAllocate() {
	return rtvManager_->Allocate();
}

void MAGISYSTEM::CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {
	rtvManager_->CreateRTVTexture2d(rtvIndex, pResource, format);
}

ID3D12DescriptorHeap* MAGISYSTEM::GetDSVDescriptorHeap() {
	return dsvManager_->GetDescriptorHeap();
}

D3D12_CPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetDSVDescriptorHandleCPU(uint32_t index) {
	return dsvManager_->GetDescriptorHandleCPU(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetDSVDescriptorHandleGPU(uint32_t index) {
	return dsvManager_->GetDescriptorHandleGPU(index);
}

uint32_t MAGISYSTEM::DSVAllocate() {
	return dsvManager_->Allocate();
}

void MAGISYSTEM::CreateDSVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {
	dsvManager_->CreateDSVTexture2d(rtvIndex, pResource, format);
}

ID3D12DescriptorHeap* MAGISYSTEM::GetSrvUavDescriptorHeap() {
	return srvuavManager_->GetDescriptorHeap();
}

D3D12_CPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetSrvUavDescriptorHandleCPU(uint32_t index) {
	return srvuavManager_->GetDescriptorHandleCPU(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetSrvUavDescriptorHandleGPU(uint32_t index) {
	return srvuavManager_->GetDescriptorHandleGPU(index);
}

uint32_t MAGISYSTEM::SrvUavAllocate() {
	return srvuavManager_->Allocate();
}

void MAGISYSTEM::CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	srvuavManager_->CreateSrvStructuredBuffer(viewIndex, pResource, numElements, structureByteStride);
}

void MAGISYSTEM::CreateSrvTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {
	srvuavManager_->CreateSrvTexture2d(srvIndex, pResource, format, mipLevels);
}

void MAGISYSTEM::CreateSrvByteAddressBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t byteSize) {
	srvuavManager_->CreateSrvByteAddressBuffer(viewIndex, pResource, byteSize);
}

void MAGISYSTEM::CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	srvuavManager_->CreateUavStructuredBuffer(viewIndex, pResource, numElements, structureByteStride);
}

ID3D12RootSignature* MAGISYSTEM::GetGraphicsRootSignature(GraphicsPipelineStateType pipelineState) {
	return graphicsPipelineManager_->GetRootSignature(pipelineState);
}

ID3D12PipelineState* MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return graphicsPipelineManager_->GetPipelineState(pipelineState, blendMode);
}

ID3D12RootSignature* MAGISYSTEM::GetComputeRootSignature(ComputePipelineStateType pipelineState) {
	return computePipelineManager_->GetRootSignature(pipelineState);
}

ID3D12PipelineState* MAGISYSTEM::GetComputePipelineState(ComputePipelineStateType pipelineState) {
	return computePipelineManager_->GetPipelineState(pipelineState);
}

ID3D12RootSignature* MAGISYSTEM::GetPostEffectRootSignature(PostEffectType pipelineState) {
	return postEffectPipelineManager_->GetRootSignature(pipelineState);
}

ID3D12PipelineState* MAGISYSTEM::GetPostEffectPipelineState(PostEffectType pipelineState, BlendMode blendMode) {
	return postEffectPipelineManager_->GetPipelineState(pipelineState, blendMode);
}

ID3D12RootSignature* MAGISYSTEM::GetShadowRootSignature(ShadowPipelineStateType pipelineState) {
	return shadowPipelineManager_->GetRootSignature(pipelineState);
}

ID3D12PipelineState* MAGISYSTEM::GetShadowPipelineState(ShadowPipelineStateType pipelineState) {
	return shadowPipelineManager_->GetPipelineState(pipelineState);
}

void MAGISYSTEM::ApplyPostEffectGrayScale() {
	PostEffectCommand command{
		.postEffectType = PostEffectType::Grayscale,
	};
	// コマンドを追加
	renderController_->AddPostEffect(command);
}

void MAGISYSTEM::ApplyPostEffectVignette(float scale, float falloff) {
	PostEffectCommand command{
		.postEffectType = PostEffectType::Vignette,
		.param = {
			.param = {scale,falloff},
		}
	};
	// コマンドを追加
	renderController_->AddPostEffect(command);
}

void MAGISYSTEM::ApplyPostEffectGaussianX(float sigma, uint32_t karnelSize) {
	// 重みを計算
	const std::array<float, 7> weight = GenerateGaussianWeights(sigma);
	// カーネルの半径を計算
	const float kernelRadius = (karnelSize - 1) * 0.5f;
	PostEffectCommand command{
		.postEffectType = PostEffectType::GaussianX,
		.param = {
			.param = {1.0f / WindowApp::kClientWidth,1.0f / WindowApp::kClientHeight,0.0f,0.0f,
				weight[0],weight[1],weight[2],weight[3],
				weight[4],weight[5],weight[6],0.0f,
				kernelRadius,
			},
		}
	};
	// コマンドを追加
	renderController_->AddPostEffect(command);
}

void MAGISYSTEM::ApplyPostEffectGaussianY(float sigma, uint32_t karnelSize) {
	// 重みを計算
	const std::array<float, 7> weight = GenerateGaussianWeights(sigma);
	// カーネルの半径を計算
	const float kernelRadius = (karnelSize - 1) * 0.5f;
	PostEffectCommand command{
		.postEffectType = PostEffectType::GaussianY,
		.param = {
			.param = {1.0f / WindowApp::kClientWidth,1.0f / WindowApp::kClientHeight,0.0f,0.0f,
				weight[0],weight[1],weight[2],weight[3],
				weight[4],weight[5],weight[6],0.0f,
				kernelRadius,
			},
		}
	};
	// コマンドを追加
	renderController_->AddPostEffect(command);
}

void MAGISYSTEM::ApplyPostEffectRadialBlur(Vector2 center, float blurWidth) {
	PostEffectCommand command{
		.postEffectType = PostEffectType::RadialBlur,
		.param = {
			.param = {center.x,center.y,0.0f,0.0f,
				blurWidth,
			},
		}
	};
	// コマンドを追加
	renderController_->AddPostEffect(command);
}

uint32_t MAGISYSTEM::LoadTexture(const std::string& fileName, bool isFullPath) {
	return textureDataCantainer_->Load(fileName, isFullPath);
}

void MAGISYSTEM::LoadNormalMapTexture(const std::string& filePath) {
	textureDataCantainer_->LoadNormalMap(filePath);
}

std::unordered_map<std::string, Texture>& MAGISYSTEM::GetTexture() {
	return textureDataCantainer_->GetTexture();
}

const DirectX::TexMetadata& MAGISYSTEM::GetTextureMetaData(const std::string& filePath) {
	return textureDataCantainer_->GetMetaData(filePath);
}

const std::unordered_map<std::string, Texture>& MAGISYSTEM::GetTextureContainer() {
	return textureDataCantainer_->GetTextureContainer();
}

uint32_t MAGISYSTEM::GetTextureIndex(const std::string& textureName) {
	const auto& textureMap = textureDataCantainer_->GetTexture();
	auto it = textureMap.find(textureName);
	assert(it != textureMap.end() && "Texture name not found in textureMap");
	return it->second.srvIndex;
}

uint32_t MAGISYSTEM::GetDefaultTextureIndex() {
	return textureDataCantainer_->GetDefaultTextureIndex();
}

PrimitiveData MAGISYSTEM::GetPrimitiveShape(const Primitive3DType& primitive3dType) {
	return primitiveDataContainer_->GetPrimitiveShapeData(primitive3dType);
}

void MAGISYSTEM::LoadModel(const std::string& modelName) {
	modelDataContainer_->Load(modelName);
}

ModelData MAGISYSTEM::FindModel(const std::string& modelName) {
	return modelDataContainer_->FindModelData(modelName);
}

void MAGISYSTEM::LoadAnimation(const std::string& animationFileName) {
	animationDataContainer_->Load(animationFileName);
}

AnimationData MAGISYSTEM::FindAnimation(const std::string& animationName) {
	return animationDataContainer_->FindAnimationData(animationName);
}

void MAGISYSTEM::LoadWaveSound(const std::string& fileName) {
	soundDataContainer_->LoadWave(fileName);
}

void MAGISYSTEM::PlayWaveSound(const std::string& fileName) {
	soundDataContainer_->PlayWave(fileName);
}


void MAGISYSTEM::PlayLoopWaveSound(const std::string& fileName) {
	soundDataContainer_->PlayWaveLoop(fileName);
}

void MAGISYSTEM::StopWaveSound(const std::string& fileName) {
	soundDataContainer_->StopWave(fileName);
}

void MAGISYSTEM::StopLoopWaveSound(const std::string& fileName) {
	soundDataContainer_->StopWaveLoop(fileName);
}

void MAGISYSTEM::LoadSceneDataFromJson(const std::string& fileName) {
	sceneDataContainer_->LoadBlenderLevelDataFromJson(fileName);
}

Transform3D* MAGISYSTEM::AddTransform3D(std::unique_ptr<Transform3D> transform) {
	return transformManager_->Add(std::move(transform));
}

std::weak_ptr<ModelRenderer> MAGISYSTEM::AddRenderer3D(std::shared_ptr<ModelRenderer> modelRenderer) {
	return renderer3DManager_->Add(std::move(modelRenderer));
}

std::weak_ptr<GameObject3D> MAGISYSTEM::AddGameObject3D(std::shared_ptr<GameObject3D> gameObjec3D, bool insertMap) {
	return gameObject3DManager_->Add(std::move(gameObjec3D), insertMap);
}

std::weak_ptr<GameObject3D> MAGISYSTEM::FindGameObject3D(const std::string& objectName) {
	return gameObject3DManager_->Find(objectName);
}

void MAGISYSTEM::TransferCamera3D(uint32_t rootParameterIndex) {
	camera3DManager_->TransferCurrentCamera(rootParameterIndex);
}

void MAGISYSTEM::TransferCurrentCamera3DFrustum(uint32_t rootParameterIndex) {
	camera3DManager_->TransferCurrentCameraFrustum(rootParameterIndex);
}

void MAGISYSTEM::ShakeCurrentCamera3D(float duration, const Vector3& intensity) {
	camera3DManager_->ShakeCurrentCamera(duration, intensity);
}

void MAGISYSTEM::ClearCamera3D() {
	camera3DManager_->Clear();
}

std::string MAGISYSTEM::CreateEmitter3D(const std::string& emitterName, const Vector3& position) {
	return emitter3DManager_->CreateEmitter(emitterName, position);
}

void MAGISYSTEM::RemoveEmitter3D(const std::string& emitterName) {
	emitter3DManager_->Remove(emitterName);
}

Emitter3D* MAGISYSTEM::FindEmitter3D(const std::string& emitterName) {
	return emitter3DManager_->Find(emitterName);
}

void MAGISYSTEM::ClearEmitter3D() {
	emitter3DManager_->Clear();
}

std::string MAGISYSTEM::CreatePrimitiveParticleGroup3D(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName) {
	return particleGroup3DManager_->CreatePrimitiveParticleGroup(particleGroupName, primitiveType, textureName);
}

std::string MAGISYSTEM::CreateStaticParticleGroup3D(const std::string& particleGroupName, const std::string& modelName) {
	return particleGroup3DManager_->CreateStaticParticleGroup(particleGroupName, modelName);
}

BaseParticleGroup3D* MAGISYSTEM::FindParticleGroup3D(const std::string& particleGraoupName) {
	return particleGroup3DManager_->Find(particleGraoupName);
}

const std::vector<std::unique_ptr<BaseParticleGroup3D>>& MAGISYSTEM::GetParticleGroupList() {
	return particleGroup3DManager_->GetParticleGroups();
}

void MAGISYSTEM::SetDirectionalLight(const DirectionalLight& directionalLight) {
	lightManager_->SetDirectionalLight(directionalLight);
}

void MAGISYSTEM::SetDirectionalLightCameraTarget(const Vector3& target) {
	lightManager_->SetDirectionalLightCameraTarget(target);
}

void MAGISYSTEM::TransferDirectionalLightCamera(uint32_t paramIndex) {
	lightManager_->TransferDirectionalLightCamera(paramIndex);
}

void MAGISYSTEM::TransferDirectionalLightFrustum(uint32_t paramIndex) {
	lightManager_->TransferDirectionalLightFrustum(paramIndex);
}

void MAGISYSTEM::DrawSprite(const SpriteData& data, const SpriteMaterialData& material) {
	spriteDrawer_->AddSprite(data, material);
}


void MAGISYSTEM::AddCamera2D(std::unique_ptr<Camera2D> newCamera2D) {
	camera2DManager_->Add(std::move(newCamera2D));
}

Camera2D* MAGISYSTEM::FindCamera2D(const std::string& cameraName) {
	return camera2DManager_->Find(cameraName);
}

void MAGISYSTEM::SetCurrentCamera2D(const std::string& cameraName) {
	camera2DManager_->SetCurrentCamera(cameraName);
}

void MAGISYSTEM::TransferCamera2D(uint32_t rootParameterIndex) {
	camera2DManager_->TransferCurrentCamera(rootParameterIndex);
}

void MAGISYSTEM::ClearCamera2D() {
	camera2DManager_->Clear();
}

std::weak_ptr<Camera3D> MAGISYSTEM::AddCamera3D(std::shared_ptr<Camera3D> newCamera3D) {
	return camera3DManager_->Add(std::move(newCamera3D));
}

void MAGISYSTEM::SetCurrentCamera3D(Camera3D* newCamera3D) {
	camera3DManager_->SetCurrentCamera(newCamera3D);
}

Camera3D* MAGISYSTEM::GetCurrentCamera3D() {
	return camera3DManager_->GetCurrentCamera();
}

void MAGISYSTEM::DrawLine3D(const Vector3& start, const Vector3& end, const Vector4& color) {
	lineDrawer3D_->AddLine(start, end, color);
}

void MAGISYSTEM::DrawTriangle3D(const Matrix4x4& worldMatrix, const TriangleData3D& data, const MaterialData3D& material) {
	triangleDrawer3D_->AddTriangle(worldMatrix, data, material);
}

void MAGISYSTEM::DrawPlane3D(const Matrix4x4& worldMatrix, const PlaneData3D& planeData, const MaterialData3D& materialData) {
	planeDrawer3D_->AddPlane(worldMatrix, planeData, materialData);
}

void MAGISYSTEM::DrawBox3D(const Matrix4x4& worldMatrix, const BoxData3D& boxData, const MaterialData3D& material) {
	boxDrawer3D_->AddBox(worldMatrix, boxData, material);
}

void MAGISYSTEM::DrawSphere3D(const Matrix4x4& worldMatrix, const SphereData3D& data, const MaterialData3D& material) {
	sphereDrawer3D_->AddSphere(worldMatrix, data, material);
}

void MAGISYSTEM::DrawRing3D(const Matrix4x4& worldMatrix, const RingData3D& data, const MaterialData3D& material) {
	ringDrawer3D_->AddRing(worldMatrix, data, material);
}

void MAGISYSTEM::DrawCylinder3D(const Matrix4x4& worldMatrix, const CylinderData3D& data, const MaterialData3D& material) {
	cylinderDrawer3D_->AddCylinder(worldMatrix, data, material);
}

void MAGISYSTEM::CreateModelDrawer(const std::string& name, const ModelData& modelData) {
	modelDrawerManager_->CreateModelDrawer(name, modelData);
}

void MAGISYSTEM::DrawModel(const std::string& name, const Matrix4x4& worldMatrix, const ModelMaterial& material) {
	modelDrawerManager_->DrawModel(name, worldMatrix, material);
}

void MAGISYSTEM::CreateSkinModelDrawer(const std::string& name, const ModelData& modelData) {
	modelDrawerManager_->CreateSkinModelDrawer(name, modelData);
}

void MAGISYSTEM::DrawSkinModel(const std::string& name, const Matrix4x4& worldMatrix, const ModelMaterial& material) {
	modelDrawerManager_->DrawSkinModel(name, worldMatrix, material);
}

void MAGISYSTEM::SetSkyBoxTextureIndex(uint32_t skyBoxTextureIndex) {
	skyBoxDrawer_->SetTextureIndex(skyBoxTextureIndex);
}

void MAGISYSTEM::AddPlaneEffect(const PlaneEffectParam& param) {
	planeEffectManager_->Add(param);
}

void MAGISYSTEM::AddGrobalDataGroup(const std::string& groupname) {
	grobalDataManager_->CreateGroup(groupname);
}

void MAGISYSTEM::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, int32_t value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void MAGISYSTEM::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, float value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void MAGISYSTEM::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, Vector3 value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void MAGISYSTEM::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, bool value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void MAGISYSTEM::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, int32_t value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

void MAGISYSTEM::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, float value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

void MAGISYSTEM::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, Vector3 value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

void MAGISYSTEM::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, bool value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

int32_t MAGISYSTEM::GetGrobalDataValueInt(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueInt(groupName, key);
}

float MAGISYSTEM::GetGrobalDataValueFloat(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueFloat(groupName, key);
}

Vector3 MAGISYSTEM::GetGrobalDataValueVector3(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueVector3(groupName, key);
}

bool MAGISYSTEM::GetGrobalDataValueBool(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueBool(groupName, key);
}

void MAGISYSTEM::ImportSceneData(const std::string& sceneDataName, bool isSceneClear) {
	sceneDataImporter_->Import(sceneDataName, isSceneClear);
}
