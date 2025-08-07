#pragma once

// C++
#include <memory>

#if defined(DEBUG) || defined(DEVELOP)
#include "LeakChecker/D3DResourceLeakChecker.h"
#endif // _DEBUG

// 
// BaseSystems
// 
#include "WindowApp/WindowApp.h"
#include "DeltaTimer/DeltaTimer.h"    
#include "Input/MAGIDirectInput/MAGIDirectInput.h"
#include "Input/MAGIXInput/MAGIXInput.h"

// 
// DirectXBaseSystems
// 
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/Fence/Fence.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

// 
// ViewManagers
// 
#include "ViewManagers/RTVManager/RTVManager.h"
#include "ViewManagers/DSVManager/DSVManager.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

// 
// DirectXRenderSystems
// 
#include "DirectX/SwapChain/SwapChain.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"


// 
// AssetContainer
// 
#include "TextureDataContainer/TextureDataContainer.h"
#include "PrimitiveShapeDataContainer/PrimitiveShapeDataContainer.h"
#include "SceneDataContainer/SceneDataContainer.h"
#include "ModelDataContainer/ModelDataContainer.h"
#include "AnimationDataContainer/AnimationDataContainer.h"
#include "SoundDataContainer/SoundDataContainer.h"

// 
// PipelineManager
// 
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "PipelineManagers/ComputePipelineManager/ComputePipelineManager.h"
#include "PipelineManagers/DefferedRenderringPipelineManager/DefferedRenderringPipelineManager.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"
#include "PipelineManagers/ShadowPipelineManager/ShadowPipelineManager.h"

// 
// ComponentManagers
// 
#include "TransformManager/TransformManager.h"
#include "Renderer3DManager/Renderer3DManager.h"

// 
// ObjectManager
// 
#include "GameObject3DManager/GameObject3DManager.h"
#include "Camera2DManager/Camera2DManager.h"
#include "Camera3DManager/Camera3DManager.h"
#include "LightManager/LightManager.h"



// 
// Drawer
// 
#include "2D/Drawer2D/SpriteDrawer/SpriteDrawer.h"

#include "3D/Drawer3D/PrimitiveDrawers/LineDrawer3D/LineDrawer3D.h"
#include "3D/Drawer3D/PrimitiveDrawers/TriangleDrawer3D/TriangleDrawer3D.h"
#include "3D/Drawer3D/PrimitiveDrawers/PlaneDrawer3D/PlaneDrawer3D.h"
#include "3D/Drawer3D/PrimitiveDrawers/BoxDrawer3D/BoxDrawer3D.h"
#include "3D/Drawer3D/PrimitiveDrawers/SphereDrawer3D/SphereDrawer3D.h"
#include "3D/Drawer3D/PrimitiveDrawers/RingDrawer3D/RingDrawer3D.h"
#include "3D/Drawer3D/PrimitiveDrawers/CylinderDrawer3D/CylinderDrawer3D.h"

#include "3D/ModelDrawerManager/ModelDrawerManager.h"

#include "3D/Drawer3D/SkyBoxDrawer/SkyBoxDrawer.h"


// 
// ParticleSystem
// 
#include "ParticleUpdater3D/ParticleUpdater3D.h"
#include "Drawer3D/ParticleDrawer3D/ParticleDrawer3D.h"


//
// Effect
//
#include "Effect3D/EffectManagers3D/PrimitiveEffectManagers3D/PlaneEffectManager/PlaneEffectManager.h"


// 
// GameManager
// 
#include "SceneManager/SceneManager.h"


//
// RenderPipelineController
//
#include "RenderPipelineController/RenderPipelineController.h"

// 
// Data入出力クラス
// 
#include "GrobalDataManager/GrobalDataManager.h"
#include "SceneDataImporter/SceneDataImporter.h"

// 
// UIクラス
// 
#include "ImGuiController/ImGuiController.h"
#include "GUI/GUI.h"

// エンジンの全機能を持つクラス
class MAGISYSTEM {
public:
	// 仮想デストラクタ
	virtual~MAGISYSTEM() = default;
	// 初期化
	virtual void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();

	// 削除フラグの立っているオブジェクトを削除
	void DeleteGarbages();

	// 実行
	void Run();

	// 終了チェック
	bool IsEndRequest() const;

public: // エンジンの機能

	// TODO: regionを使わない方法を検討する

#pragma region WindowAppの機能
	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	static HWND GetWindowHandle();
#pragma endregion

#pragma region DeltaTimerの機能
	/// <summary>
	/// デルタタイムを取得
	/// </summary>
	/// <returns>デルタタイムを返す</returns>
	static float GetDeltaTime();

#pragma endregion

#pragma region DirectInputの機能
	/// <summary>
	/// 指定番号のキーボードが押されているかどうかを返します
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>指定したキーが押されているかどうか</returns>
	static bool PushKey(BYTE keyNumber);
	// キーを押した
	static bool TriggerKey(BYTE keyNumber);
	// キーを押し続けている
	static bool HoldKey(BYTE keyNumber);
	// キーを離した
	static bool ReleaseKey(BYTE keyNumber);

	// マウスボタンを押している
	static bool PushMouseButton(MouseButton mouseButton);
	// マウスボタンを押した
	static bool TriggerMouseButton(MouseButton mouseButton);
	// マウスボタンを押し続けている
	static bool HoldMouseButton(MouseButton mouseButton);
	// マウスボタンを離した
	static bool ReleaseMouseButton(MouseButton mouseButton);

	// マウス移動の入力を取得
	static LONG GetMouseMoveDeltaX();
	static LONG GetMouseMoveDeltaY();

	// マウスホイールの入力を取得
	static int64_t GetMouseWheelDelta();
#pragma endregion

#pragma region XInputの機能
	// パッドの接続状況を確認
	static bool IsPadConnected(int controllerID);

	// コントローラのボタン入力
	static bool PushButton(int controllerID, int buttonNumber);
	static bool TriggerButton(int controllerID, int buttonNumber);
	static bool HoldButton(int controllerID, int buttonNumber);
	static bool ReleaseButton(int controllerID, int buttonNumber);

	// スティックの入力を取得
	static Vector2 GetLeftStick(int controllerID);
	static Vector2 GetRightStick(int controllerID);

	// トリガーの位置取得
	static float GetLeftTrigger(int controllerID);
	static float GetRightTrigger(int controllerID);

	// 方向キー
	static bool IsPadUp(int controllerID);
	static bool IsPadRight(int controllerID);
	static bool IsPadDown(int controllerID);
	static bool IsPadLeft(int controllerID);

	// デッドゾーンの設定
	static void SetDeadZone(int deadZone);
	// デッドゾーンの取得
	static int GetDeadZone();

	// 振動開始
	static void StartPadVibration(int controllerID, float duration, float leftPower, float rightPower);
	// 振動終了
	static void StopPadVibration(int controllerID);

#pragma endregion

#pragma region DXGIの機能
	// デバイス取得
	static ID3D12Device* GetDirectXDevice();
	// バッファリソースを作成
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes, bool isUav = false);
	// DepthStencilTexリソースの作成
	static ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height, DXGI_FORMAT format);

#pragma endregion

#pragma region DirectXCommandの機能
	// コマンドリストを取得
	static ID3D12GraphicsCommandList* GetDirectXCommandList();
	// 新型のコマンドリストを取得
	static ID3D12GraphicsCommandList6* GetDirectXCommandList6();
	// コマンドのクローズと実行
	static void KickCommand();
	// コマンドのリセット
	static void ResetCommand();
#pragma endregion

#pragma region Fenceの機能
	// GPUを待機
	static void WaitGPU();
#pragma endregion

#pragma region RTVManagerの機能
	// RTVのディスクリプタヒープを取得
	static ID3D12DescriptorHeap* GetRTVDescriptorHeap();
	// RTVのCPUディスクリプタハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleCPU(uint32_t index);
	// RTVのGPUディスクリプタハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleGPU(uint32_t index);
	// RTVIndex割り当て関数
	static uint32_t RTVAllocate();
	// Texture2D用のRTVの作成
	static void CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);
#pragma endregion

#pragma region DSVManagerの機能
	// DSVのディスクリプタヒープを取得
	static ID3D12DescriptorHeap* GetDSVDescriptorHeap();
	// DSVのCPUディスクリプタハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetDSVDescriptorHandleCPU(uint32_t index);
	// DSVのGPUディスクリプタハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetDSVDescriptorHandleGPU(uint32_t index);
	// DSVIndex割り当て関数
	static uint32_t DSVAllocate();
	// Texture2D用のDSV作成
	static void CreateDSVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);
#pragma endregion


#pragma region SRVUAVManagerの機能
	// SRVUAVのディスクリプタヒープを取得
	static ID3D12DescriptorHeap* GetSrvUavDescriptorHeap();

	// SRVのCPUディスクリプタハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvUavDescriptorHandleCPU(uint32_t index);
	// SRVのGPUディスクリプタハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvUavDescriptorHandleGPU(uint32_t index);
	// Allocate
	static uint32_t SrvUavAllocate();
	// StructuredBuffer用のsrv作成
	static void CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
	// Texure2D用のSrv作成
	static void CreateSrvTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	// RawBuffer用のsrv作成
	static void CreateSrvByteAddressBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t byteSize);
	// StructuredBuffer用のUAV作成
	static void CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
#pragma endregion

#pragma region GraphicsPipelineManager
	// ルートシグネイチャ取得関数
	static ID3D12RootSignature* GetGraphicsRootSignature(GraphicsPipelineStateType pipelineState);
	// パイプライン取得関数
	static ID3D12PipelineState* GetGraphicsPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);

#pragma endregion

#pragma region ComputePipelineManager
	// ルートシグネイチャ取得関数
	static ID3D12RootSignature* GetComputeRootSignature(ComputePipelineStateType pipelineState);
	// パイプライン取得関数
	static ID3D12PipelineState* GetComputePipelineState(ComputePipelineStateType pipelineState);
#pragma endregion

#pragma region PostEffectPipelineManager
	// ルートシグネイチャ取得関数
	static ID3D12RootSignature* GetPostEffectRootSignature(PostEffectType pipelineState);
	// パイプライン取得関数
	static ID3D12PipelineState* GetPostEffectPipelineState(PostEffectType pipelineState, BlendMode blendMode);
#pragma endregion

#pragma region ShadowPipelineManager
	// ルートシグネイチャ取得関数
	static ID3D12RootSignature* GetShadowRootSignature(ShadowPipelineStateType pipelineState);
	// パイプライン取得関数
	static ID3D12PipelineState* GetShadowPipelineState(ShadowPipelineStateType pipelineState);
#pragma endregion

#pragma region RenderController
	// シーンにグレースケールをかける
	static void ApplyPostEffectGrayScale();
	// シーンにビネットをかける
	static void ApplyPostEffectVignette(float scale, float falloff);
	// シーンにX軸ガウスぼかしをかける(karnelSizeは、3,5,7,9,11,13)
	static void ApplyPostEffectGaussianX(float sigma, uint32_t karnelSize);
	// シーンにY軸ガウスぼかしをかける(karnelSizeは、3,5,7,9,11,13)
	static void ApplyPostEffectGaussianY(float sigma, uint32_t karnelSize);
	// シーンに集中ぼかしをかける
	static void ApplyPostEffectRadialBlur(Vector2 center, float blurWidth);
	// シーンに輝度比較アウトラインをかける
	static void ApplyPostEffectLuminanceOutline();
	// シーンに深度比較アウトラインをかける
	static void ApplyPostEffectDepthOutline();
	// シーンに深度と法線比較アウトラインをかける
	static void ApplyPostEffectDepthNormalOutline();
	// シーンにランダムノイズのポストエフェクトをかける
	static void ApplyPostEffectRandom();

#pragma endregion

#pragma region TextureDataContainer
	/// <summary>
	/// 画像読み込み関数
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="isFullPath">フルパス指定かどうか</param>
	/// <returns>テクスチャのSRVインデックス</returns>
	static uint32_t LoadTexture(const std::string& fileName, bool isFullPath = false);
	// 法線マップ画像の読み込み
	static void LoadNormalMapTexture(const std::string& filePath);
	// テクスチャの取得
	static std::unordered_map<std::string, Texture>& GetTexture();
	// メタデータ取得
	static const DirectX::TexMetadata& GetTextureMetaData(const std::string& filePath);
	// テクスチャコンテナを渡す
	static const std::unordered_map<std::string, Texture>& GetTextureContainer();
	// テクスチャのインデックスを渡す
	static uint32_t GetTextureIndex(const std::string& textureName);
	// エンジンのデフォルトテクスチャインデクス(UVChecker)を渡す
	static uint32_t GetDefaultTextureIndex();

#pragma endregion

#pragma region PrimitiveShapeDataContainer
	// 形状の取得
	static PrimitiveData GetPrimitiveShape(const Primitive3DType& primitive3dType);
#pragma endregion

#pragma region ModelDataContainer
	// モデルの読み込み
	static void LoadModel(const std::string& modelName);
	// 読み込み済みモデル検索
	static ModelData FindModel(const std::string& modelName);
#pragma endregion

#pragma region AnimationDataContainer
	// アニメーションの読み込み
	static void LoadAnimation(const std::string& animationFileName);
	// 読み込み済みアニメーションの検索
	static AnimationData FindAnimation(const std::string& animationName);
#pragma endregion

#pragma region SoundDataContainer
	// サウンドの読み込み
	static void LoadWaveSound(const std::string& fileName);
	// サウンドの再生
	static void PlayWaveSound(const std::string& fileName);
	// サウンドループ再生
	static void PlayLoopWaveSound(const std::string& fileName);
	// サウンドの停止
	static void StopWaveSound(const std::string& fileName);
	// サウンドループ再生停止
	static void StopLoopWaveSound(const std::string& fileName);
#pragma endregion

#pragma region SceneDataContainer
	// シーンデータの読み込み
	static void LoadSceneDataFromJson(const std::string& fileName);
#pragma endregion


#pragma region TransformComponent
	// トランスフォーム追加
	static Transform3D* AddTransform3D(std::unique_ptr<Transform3D> transform);
#pragma endregion

#pragma region Renderer3DComponent
	// モデルのレンダラー追加
	static std::weak_ptr<ModelRenderer> AddRenderer3D(std::shared_ptr<ModelRenderer> modelRenderer);

#pragma endregion

#pragma region GameObject3DManager
	// ゲームオブジェクト3Dを追加
	static std::weak_ptr<GameObject3D> AddGameObject3D(std::shared_ptr<GameObject3D> gameObjec3D, bool insertMap = true);
	// ゲームオブジェクト3Dを検索して取得
	static std::weak_ptr<GameObject3D> FindGameObject3D(const std::string& objectName);
	// ゲームオブジェクトを全削除
	static void ClearGameObject3D();
#pragma endregion

#pragma region Camera2DManager
	// 2Dカメラの追加
	static void AddCamera2D(std::unique_ptr<Camera2D> newCamera2D);
	// 2Dカメラの取得
	static Camera2D* FindCamera2D(const std::string& cameraName);
	// 使用する2Dカメラのセット
	static void SetCurrentCamera2D(const std::string& cameraName);
	// 2Dカメラの転送
	static void TransferCamera2D(uint32_t rootParameterIndex);
	// 2Dカメラ全削除
	static void ClearCamera2D();
#pragma endregion

#pragma region Camera3DManager
	// 3Dカメラの追加
	static std::weak_ptr<Camera3D> AddCamera3D(std::shared_ptr<Camera3D> newCamera3D);
	// 使用する3Dカメラのセット
	static void SetCurrentCamera3D(Camera3D* camera3D);
	// 使用している3Dカメラの取得
	static Camera3D* GetCurrentCamera3D();
	// 3Dカメラの転送
	static void TransferCamera3D(uint32_t rootParameterIndex);
	// Frustumの転送
	static void TransferCurrentCamera3DFrustum(uint32_t rootParameterIndex);
	// カメラシェイク
	static void ShakeCurrentCamera3D(float duration, const Vector3& intensity);
	// 3Dカメラ全削除
	static void ClearCamera3D();
#pragma endregion

#pragma region LightManager
	// DirectionalLightをセット
	static void SetDirectionalLight(const DirectionalLight& directionalLight);
	// DirectionalLightのカメラターゲットをセット
	static void SetDirectionalLightCameraTarget(const Vector3& target);
	// ライトカメラを転送
	static void TransferDirectionalLightCamera(uint32_t paramIndex);
	// ライトのフラスタムを転送
	static void TransferDirectionalLightFrustum(uint32_t paramIndex);
#pragma endregion

#pragma region SpriteDrawer
	// スプライト描画
	static void DrawSprite(const SpriteData& data, const SpriteMaterialData& material);
#pragma endregion

#pragma region LineDrawer3D
	// ライン描画
	static void DrawLine3D(const Vector3& start, const Vector3& end, const Vector4& color);
	// ラインでAABBを描画
	static void DrawLineAABB(const Vector3& min, const Vector3& max, const Vector4& color);
#pragma endregion

#pragma region TriangleDrawer3D
	// 三角形描画
	static void DrawTriangle3D(
		const Matrix4x4& worldMatrix,
		const TriangleData3D& data,
		const MaterialData3D& material
	);
#pragma endregion

#pragma region PlaneDrawer3D
	// 板ポリ描画
	static void DrawPlane3D(
		const Matrix4x4& worldMatrix,
		const PlaneData3D& planeData,
		const MaterialData3D& materialData
	);

#pragma endregion

#pragma region BoxDrawer3D
	// ボックス描画
	static void DrawBox3D(
		const Matrix4x4& worldMatrix,
		const BoxData3D& boxData,
		const MaterialData3D&
	);
#pragma endregion


#pragma region SphereDrawer3D
	// 球体描画
	static void DrawSphere3D(
		const Matrix4x4& worldMatrix,
		const SphereData3D& data = SphereData3D{},
		const MaterialData3D& material = MaterialData3D{}
	);
#pragma endregion

#pragma region RingDrawer3D
	// リング描画
	static void DrawRing3D(
		const Matrix4x4& worldMatrix,
		const RingData3D& data,
		const MaterialData3D& material
	);
#pragma endregion

#pragma region CylinderDrawer3D
	// シリンダー描画
	static void DrawCylinder3D(
		const Matrix4x4& worldMatrix,
		const CylinderData3D& data,
		const MaterialData3D& material
	);
#pragma endregion

#pragma region ModelDrawerManager
	// ModelDrawer追加
	static void CreateModelDrawer(
		const std::string& name,
		const ModelData& modelData
	);

	// モデル描画
	static void DrawModel(
		const std::string& name,
		const Matrix4x4& worldMatrix,
		const ModelMaterial& material
	);

	// SkinModelDrawer追加
	static void CreateSkinModelDrawer(
		const std::string& name,
		const ModelData& modelData
	);

	// SkinModelDrawerのアニメーションを再生　戻り値は再生中かどうか
	static bool ApplyAnimationSkinModel(
		const std::string& name,
		const AnimationData& animation,
		float animationTime,
		bool loopFrag
	);

	// スキンモデル描画
	static void DrawSkinModel(
		const std::string& name,
		const Matrix4x4& worldMatrix,
		const ModelMaterial& material
	);

#pragma endregion

#pragma region SkyBoxDrawer
	// スカイボックスのテクスチャインデックスをセット
	static void SetSkyBoxTextureIndex(uint32_t skyBoxTextureIndex);

#pragma endregion

#pragma region ParticleSystem
	// パーティクル発生
	static void EmitParticle(const GPUParticleEmitData& emitData);
#pragma endregion


#pragma region PlaneEffectManager
	// 板ポリエフェクトを追加
	static void AddPlaneEffect(const PlaneEffectParam& param);

#pragma endregion


#pragma region CollisionManager


#pragma endregion

#pragma region DataIO


#pragma endregion 

#pragma region GrobalDataManager
	// グループの追加
	static void AddGrobalDataGroup(const std::string& groupname);
	// アイテムの追加
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, int32_t value);
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, float value);
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, Vector3 value);
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, bool value);

	// グループとキーから値をセット
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, int32_t value);
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, float value);
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, Vector3 value);
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, bool value);

	// グループとキーから値の取得
	static int32_t GetGrobalDataValueInt(const std::string& groupName, const std::string& key);
	static float GetGrobalDataValueFloat(const std::string& groupName, const std::string& key);
	static Vector3 GetGrobalDataValueVector3(const std::string& groupName, const std::string& key);
	static bool GetGrobalDataValueBool(const std::string& groupName, const std::string& key);
#pragma endregion

#pragma region SceneDataImporter
	// シーンデータをインポート
	static void ImportSceneData(const std::string& sceneDataName, bool isSceneClear = true);

#pragma endregion


#pragma region GUI


#pragma endregion

private: // メンバ変数
	// 終了リクエスト
	bool endRequest_ = false;
	// DX12Ultimateに対応したデバイスかどうか(ASやMSを使えるかどうか)
	bool isSupportDX12Ultimate_ = false;

protected:
#if defined(DEBUG) || defined(DEVELOP)
	static std::unique_ptr<D3DResourceLeakChecker> leakCheck_;
#endif // _DEBUG

	// 
	// BaseSystems
	// 
	static std::unique_ptr<WindowApp> windowApp_;
	static std::unique_ptr<DeltaTimer> deltaTimer_;
	static std::unique_ptr<MAGIDirectInput> directInput_;
	static std::unique_ptr<MAGIXInput> xInput_;

	// 
	// DirectXBaseSystems
	// 
	static std::unique_ptr<DXGI> dxgi_;
	static std::unique_ptr<DirectXCommand> directXCommand_;
	static std::unique_ptr<Fence> fence_;
	static std::unique_ptr<ShaderCompiler> shaderCompiler_;

	// 
	// ViewManagers
	// 
	static std::unique_ptr<RTVManager> rtvManager_;
	static std::unique_ptr<DSVManager> dsvManager_;
	static std::unique_ptr<SRVUAVManager> srvuavManager_;

	// 
	// DirectXRenderSystems
	// 
	static std::unique_ptr<Viewport> viewport_;
	static std::unique_ptr<ScissorRect> scissorRect_;
	static std::unique_ptr<DepthStencil> depthStencil_;
	static std::unique_ptr<SwapChain> swapChain_;

	//
	// PipelineManager
	//
	static std::unique_ptr<GraphicsPipelineManager> graphicsPipelineManager_;
	static std::unique_ptr<ComputePipelineManager> computePipelineManager_;
	static std::unique_ptr<DefferedRenderringPipelineManager> defferedRenderringPipelineManager_;
	static std::unique_ptr<PostEffectPipelineManager> postEffectPipelineManager_;
	static std::unique_ptr<ShadowPipelineManager> shadowPipelineManager_;

	// 
	// AssetContainer
	// 
	static std::unique_ptr<TextureDataContainer> textureDataCantainer_;
	static std::unique_ptr<PrimitiveShapeDataContainer> primitiveDataContainer_;
	static std::unique_ptr<ModelDataContainer> modelDataContainer_;
	static std::unique_ptr<AnimationDataContainer> animationDataContainer_;
	static std::unique_ptr<SoundDataContainer> soundDataContainer_;
	static std::unique_ptr<SceneDataContainer> sceneDataContainer_;

	//
	// ComponentManagers
	//
	static std::unique_ptr<TransformManager> transformManager_;
	static std::unique_ptr<Renderer3DManager> renderer3DManager_;

	//
	// ObjectManager
	//
	static std::unique_ptr<GameObject3DManager> gameObject3DManager_;
	static std::unique_ptr<Camera2DManager> camera2DManager_;
	static std::unique_ptr<Camera3DManager> camera3DManager_;
	static std::unique_ptr<LightManager> lightManager_;

	// 
	// Drawer
	// 
	static std::unique_ptr<SpriteDrawer> spriteDrawer_;

	static std::unique_ptr<LineDrawer3D> lineDrawer3D_;
	static std::unique_ptr<TriangleDrawer3D> triangleDrawer3D_;
	static std::unique_ptr<PlaneDrawer3D> planeDrawer3D_;
	static std::unique_ptr<BoxDrawer3D> boxDrawer3D_;
	static std::unique_ptr<SphereDrawer3D> sphereDrawer3D_;
	static std::unique_ptr<RingDrawer3D> ringDrawer3D_;
	static std::unique_ptr<CylinderDrawer3D> cylinderDrawer3D_;
	static std::unique_ptr<ModelDrawerManager> modelDrawerManager_;
	static std::unique_ptr<SkyBoxDrawer> skyBoxDrawer_;


	// 
	// ParticleSystem
	// 
	static std::unique_ptr<ParticleUpdater3D> particleUpdater3D_;
	static std::unique_ptr<ParticleDrawer3D> particleDrawer3D_;

	//
	// EffectManager
	//
	static std::unique_ptr<PlaneEffectManager> planeEffectManager_;

	//
	// AppSystems
	//
	static std::unique_ptr<RenderController> renderController_;

	// 
	// GameManager
	// 
	static std::unique_ptr<SceneManager<GameData>> sceneManager_;


	//
	// Data入出力クラス
	//
	static std::unique_ptr<GrobalDataManager> grobalDataManager_;
	static std::unique_ptr<SceneDataImporter> sceneDataImporter_;

	//
	// UIクラス
	//
	static std::unique_ptr<ImGuiController> imguiController_;
	static std::unique_ptr<GUI> gui_;

};

// シーンで使う一部の機能を持つクラス
class MAGI {
public:

};