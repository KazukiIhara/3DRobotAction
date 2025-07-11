#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "SimpleAnimation/SimpleAnimation.h"

#include "3D/Cameras3D/ThirdPersonCamera/ThirdPersonCamera.h"

using namespace MAGIUtility;

// サンプルシーン
template <typename Data>
class SampleScene : public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~SampleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// カメラ
	std::weak_ptr<Camera3D> mainCamera_;
	std::unique_ptr<Camera2D> sceneCamera2D_ = nullptr;


	// 
	// ポストエフェクト用変数
	// 

	float vignetteScale_ = 16.0f;
	float vignetteFalloff_ = 0.8f;

	float gaussianSigma_ = 0.5f;

	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;

	bool isOnGrayscale_ = false;
	bool isOnGaussian_ = false;
	bool isOnVignette_ = false;
	bool isRadialBlur_ = false;

	// DirectionalLight
	DirectionalLight directionalLight_{};

	// Paradin
	Transform3D* paradinTrans_ = nullptr;

	// BrainStem
	static const uint32_t brainStemNum_ = 3;
	std::array<Transform3D*, brainStemNum_> brainStemTrans_ = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {

	//
	// アセットのロード
	//

	// テクスチャ
	MAGISYSTEM::LoadTexture("pronama_chan.png");
	MAGISYSTEM::LoadTexture("gradationLine.png");
	uint32_t skyBoxTexutreIndex = MAGISYSTEM::LoadTexture("kloppenheim_06_puresky_2k.dds");

	// サウンド
	MAGISYSTEM::LoadWaveSound("Alarm01.wav");
	MAGISYSTEM::LoadWaveSound("coin.wav");

	// モデル
	MAGISYSTEM::LoadModel("teapot");

	MAGISYSTEM::LoadModel("StageObj0");
	MAGISYSTEM::CreateModelDrawer("StageObj0", MAGISYSTEM::FindModel("StageObj0"));

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));

	MAGISYSTEM::LoadModel("Paradin");
	MAGISYSTEM::CreateSkinModelDrawer("Paradin", MAGISYSTEM::FindModel("Paradin"));

	MAGISYSTEM::LoadModel("BrainStem");
	MAGISYSTEM::CreateSkinModelDrawer("BrainStem", MAGISYSTEM::FindModel("BrainStem"));

	// アニメーション
	MAGISYSTEM::LoadAnimation("Paradin_Walking");
	MAGISYSTEM::LoadAnimation("BrainStem");

	//
	// オブジェクトの作成
	//

	// 踊ってるやつ作成

	for (uint32_t i = 0; i < brainStemNum_; i++) {
		std::unique_ptr<Transform3D> brainStemTransform = std::make_unique<Transform3D>(Vector3(float(i), 0.0f, 0.0f));
		brainStemTrans_[i] = MAGISYSTEM::AddTransform3D(std::move(brainStemTransform));
	}

	// プレイヤー作成
	std::unique_ptr<Transform3D> paradinTransform = std::make_unique<Transform3D>();
	paradinTrans_ = MAGISYSTEM::AddTransform3D(std::move(paradinTransform));

	// シーンカメラ作成
	std::shared_ptr<ThirdPersonCamera> mainCamera = std::make_shared<ThirdPersonCamera>("MainCamera");
	// 現在のカメラに設定
	mainCamera->ApplyCurrent();
	// 追従対象を設定
	mainCamera->SetTargetTransform(paradinTrans_);
	// マネージャに追加
	mainCamera_ = MAGISYSTEM::AddCamera3D(std::move(mainCamera));

	// 2Dカメラ作成
	sceneCamera2D_ = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// スカイボックスの設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	// シーンデータをインポート
	MAGISYSTEM::LoadSceneDataFromJson("SceneData");
	MAGISYSTEM::ImportSceneData("SceneData");

	// 平行光源の設定
	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("GrayscaleParamater");
	ImGui::Checkbox("On", &isOnGrayscale_);
	ImGui::End();

	ImGui::Begin("VignetteParamater");
	ImGui::Checkbox("On", &isOnVignette_);
	ImGui::DragFloat("Scale", &vignetteScale_, 0.01f);
	ImGui::DragFloat("Falloff", &vignetteFalloff_, 0.01f);
	ImGui::End();

	ImGui::Begin("GaussianBlurParamater");
	ImGui::Checkbox("On", &isOnGaussian_);
	ImGui::DragFloat("Sigma", &gaussianSigma_, 0.01f);
	ImGui::End();

	ImGui::Begin("RadialBlurParamater");
	ImGui::Checkbox("On", &isRadialBlur_);
	ImGui::DragFloat2("Center", &radialBlurCenter_.x, 0.01f);
	ImGui::DragFloat("BlurWidth", &radialBlurWidth_, 0.001f);
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	ImGui::DragFloat3("Direction", &directionalLight_.direction.x, 0.01f);
	directionalLight_.direction = MAGIMath::Normalize(directionalLight_.direction);
	ImGui::DragFloat("Intensity", &directionalLight_.intensity, 0.01f);
	ImGui::ColorEdit3("Color", &directionalLight_.color.x);
	ImGui::End();

	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// ポストエフェクトをかける
	if (isOnGrayscale_) {
		MAGISYSTEM::ApplyPostEffectGrayScale();
	}
	if (isOnGaussian_) {
		MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
		MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
	}
	if (isOnVignette_) {
		MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	}
	if (isRadialBlur_) {
		MAGISYSTEM::ApplyPostEffectRadialBlur(radialBlurCenter_, radialBlurWidth_);
	}

}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	for (uint32_t i = 0; i < brainStemNum_; i++) {
		MAGISYSTEM::DrawSkinModel("BrainStem", brainStemTrans_[i]->GetWorldMatrix(), ModelMaterial{});
	}
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
