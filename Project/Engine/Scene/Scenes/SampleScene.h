#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "SimpleAnimation/SimpleAnimation.h"

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
	std::weak_ptr<Camera3D> sceneCamera_;
	std::unique_ptr<Camera2D> sceneCamera2D_ = nullptr;
	float yaw_ = 0.0f;
	float pitch_ = -0.5f;

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

	std::weak_ptr<GameObject3D> teapot_;
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

	// モデル
	MAGISYSTEM::LoadModel("teapot");

	// サウンド
	MAGISYSTEM::LoadWaveSound("Alarm01.wav");
	MAGISYSTEM::LoadWaveSound("coin.wav");

	//
	// オブジェクトの作成
	//

	// カメラ

	// シーンカメラ作成
	std::shared_ptr<Camera3D> sceneCamera = std::make_shared<Camera3D>();
	// マネージャに追加
	MAGISYSTEM::AddCamera3D("SceneCamera", std::move(sceneCamera));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera");
	sceneCamera_ = MAGISYSTEM::FindCamera3D("SceneCamera");

	// 2Dカメラ作成
	sceneCamera2D_ = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// スカイボックスの設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	// ModelDrawer
	MAGISYSTEM::CreateModelDrawer("teapot", MAGISYSTEM::FindModel("teapot"));

	std::shared_ptr<ModelRenderer> teapot = std::make_shared<ModelRenderer>("teapot", "teapot");

	std::shared_ptr<GameObject3D> object = std::make_shared<GameObject3D>("teapot", Vector3(1.0f, 0.0f, 0.0f));
	object->AddModelRenderer(std::move(teapot));

	teapot_ = MAGISYSTEM::AddGameObject3D(std::move(object));
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("ObjectManagerTest");
	if (ImGui::Button("DeleteTeapot")) {
		if (auto obj = teapot_.lock()) {
			if (auto rdr = obj->GetModelRenderer("teapot").lock()) {
				rdr->SetIsRender(!rdr->GetIsRender());
			}
		}
	}
	ImGui::End();

	ImGui::Begin("SceneImport");
	if (ImGui::Button("Import")) {
		MAGISYSTEM::LoadSceneDataFromJson("SceneData");
		MAGISYSTEM::ImportSceneData("SceneData");
	}
	ImGui::End();

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

	ImGui::Begin("SceneCamera");
	ImGui::DragFloat("Yaw", &yaw_, 0.01f);
	ImGui::DragFloat("Pitch", &pitch_, 0.01f);
	ImGui::End();

	if (auto sc = sceneCamera_.lock()) {
		sc->SetYaw(yaw_);
		sc->SetPitch(pitch_);
	}


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

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
