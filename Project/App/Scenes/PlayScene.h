#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"
#include "GameObject/Ground/Ground.h"
#include "Cameras3D/ThirdPersonCamera/ThirdPersonCamera.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
/// <typeparam name="Data"></typeparam>
template <typename Data>
class PlayScene :public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~PlayScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// カメラ
	std::weak_ptr<Camera3D> mainCamera_;

	// DirectionalLight
	DirectionalLight directionalLight_{};

	//----------------------------------------- 
	// シーンオブジェクト
	//-----------------------------------------

	// プレイヤー
	std::unique_ptr<Player> player_;

	std::unique_ptr<Ground> ground_ = nullptr;

	// ポストエフェクトの用の変数
	float vignetteScale_ = 18.0f;
	float vignetteFalloff_ = 0.8f;
	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;
	float gaussianSigma_ = 0.5f;

	// 
	// デバッグ用
	// 


	PlaneData3D planeData_;
	MaterialData3D planeMaterial_;
};

template<typename Data>
inline void PlayScene<Data>::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

	// シーンカメラ作成
	std::shared_ptr<Camera3D> sceneCamera = std::make_shared<Camera3D>();
	// マネージャに追加
	MAGISYSTEM::AddCamera3D("SceneCamera", std::move(sceneCamera));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera");

	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));

	//-------------------------------------------------------
	// アセットのロード
	//-------------------------------------------------------
	uint32_t skyBoxTexutreIndex = MAGISYSTEM::LoadTexture("kloppenheim_06_puresky_2k.dds");

	//===================================
	// テクスチャのロード
	//===================================

	// 円形テクスチャ
	MAGISYSTEM::LoadTexture("Circle2.png");

	// ホワイトテクスチャ
	MAGISYSTEM::LoadTexture("white.png");

	// 靄用
	MAGISYSTEM::LoadTexture("smoke.png");
	// リング用
	MAGISYSTEM::LoadTexture("gradation.png");
	// 粒子用
	MAGISYSTEM::LoadTexture("star.png");

	// エミッターとパーティクルを作成
	MAGISYSTEM::CreateEmitter3D("Star", Vector3(0.0f, 0.0f, 0.0f));
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("Star", Primitive3DType::Plane, "star.png");

	// エミッターとパーティクルを作成
	MAGISYSTEM::CreateEmitter3D("Haze", Vector3(0.0f, 0.0f, 0.0f));
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("Haze", Primitive3DType::Plane, "smoke.png");

	//===================================
	// モデルのロード
	//===================================

	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::CreateModelDrawer("teapot", MAGISYSTEM::FindModel("teapot"));

	//-------------------------------------------------------
	// シーン固有の初期化処理
	//-------------------------------------------------------

	// スカイボックスを設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	// プレイヤー作成
	player_ = std::make_unique<Player>();

	// 地面作成
	ground_ = std::make_unique<Ground>();

	// ゲームシーン用追尾カメラ作成
	std::shared_ptr<ThirdPersonCamera> mainCamera = std::make_shared<ThirdPersonCamera>();
	mainCamera->SetTargetTransform(player_->GetMechCore()->GetGameObject().lock()->GetTransform());
	mainCamera_ = MAGISYSTEM::AddCamera3D("MainCamera", std::move(mainCamera));
	// カメラ設定
	MAGISYSTEM::SetCurrentCamera3D("MainCamera");

}

template<typename Data>
inline void PlayScene<Data>::Update() {
	ImGui::Begin("VignetteParamater");
	ImGui::DragFloat("Scale", &vignetteScale_, 0.01f);
	ImGui::DragFloat("Falloff", &vignetteFalloff_, 0.01f);
	ImGui::End();

	ImGui::Begin("GaussianBlurParamater");
	ImGui::DragFloat("Sigma", &gaussianSigma_, 0.01f);
	ImGui::End();

	// ライト変数
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// 床更新
	ground_->Update();

	// プレイヤー更新
	player_->Update();

	// ポストエフェクト適用
	MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
}

template<typename Data>
inline void PlayScene<Data>::Draw() {
	// 床描画
	ground_->Draw();

	// プレイヤーにまつわるもの描画
	player_->Draw();

}

template<typename Data>
inline void PlayScene<Data>::Finalize() {

}
