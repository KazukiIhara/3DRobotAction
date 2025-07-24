#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "SimpleAnimation/SimpleAnimation.h"

#include "3D/Cameras3D/ThirdPersonCamera/ThirdPersonCamera.h"

using namespace MAGIUtility;

enum class ParadinState {
	Idle,
	Walk,
	AtkSword,
	AtkKick,
};

// サンプルシーン
template <typename Data>
class SampleScene: public BaseScene<Data> {
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
	float vignetteFalloff_ = 0.4f;

	float gaussianSigma_ = 0.4f;

	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;

	bool isOnGrayscale_ = false;
	bool isOnGaussian_ = true;
	bool isOnVignette_ = true;
	bool isRadialBlur_ = false;

	// DirectionalLight
	DirectionalLight directionalLight_{};

	// Paradin
	Transform3D* paradinTrans_ = nullptr;
	float paradinAnimationT_ = 0.0f;
	ParadinState curParadinState_ = ParadinState::Idle;
	float paradinSpeed_ = 2.0f;

	// BrainStem
	static const uint32_t brainStemNum_ = 3;
	std::array<Transform3D*, brainStemNum_> brainStemTrans_ = nullptr;
	float brainStemT_ = 0.0f;
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
	MAGISYSTEM::LoadAnimation("Paradin_Idle");
	MAGISYSTEM::LoadAnimation("Paradin_Attack_Sword_0");
	MAGISYSTEM::LoadAnimation("Paradin_Attack_Kick_0");
	MAGISYSTEM::LoadAnimation("BrainStem");

	//
	// オブジェクトの作成
	//

	// 踊ってるやつ作成

	for (uint32_t i = 0; i < brainStemNum_; i++) {
		std::unique_ptr<Transform3D> brainStemTransform = std::make_unique<Transform3D>(Vector3(float(i * 2), 0.0f, 2.0f));
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

	//ImGui::Begin("GrayscaleParamater");
	//ImGui::Checkbox("On", &isOnGrayscale_);
	//ImGui::End();

	//ImGui::Begin("VignetteParamater");
	//ImGui::Checkbox("On", &isOnVignette_);
	//ImGui::DragFloat("Scale", &vignetteScale_, 0.01f);
	//ImGui::DragFloat("Falloff", &vignetteFalloff_, 0.01f);
	//ImGui::End();

	//ImGui::Begin("GaussianBlurParamater");
	//ImGui::Checkbox("On", &isOnGaussian_);
	//ImGui::DragFloat("Sigma", &gaussianSigma_, 0.01f);
	//ImGui::End();

	//ImGui::Begin("RadialBlurParamater");
	//ImGui::Checkbox("On", &isRadialBlur_);
	//ImGui::DragFloat2("Center", &radialBlurCenter_.x, 0.01f);
	//ImGui::DragFloat("BlurWidth", &radialBlurWidth_, 0.001f);
	//ImGui::End();

	//ImGui::Begin("DirectionalLight");
	//ImGui::DragFloat3("Direction", &directionalLight_.direction.x, 0.01f);
	//directionalLight_.direction = MAGIMath::Normalize(directionalLight_.direction);
	//ImGui::DragFloat("Intensity", &directionalLight_.intensity, 0.01f);
	//ImGui::ColorEdit3("Color", &directionalLight_.color.x);
	//ImGui::End();

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

	MAGISYSTEM::ApplyPostffectDepthOutline();

	// パラディン操作
	Vector2 ls{};
	if (MAGISYSTEM::IsPadConnected(0)) {
		ls = MAGISYSTEM::GetLeftStick(0);
	}
	bool attackButton = MAGISYSTEM::TriggerButton(0, ButtonR);

	Vector3 velocity{};
	switch (curParadinState_) {
		case ParadinState::Idle:
		{
			if (Length(ls)) {
				curParadinState_ = ParadinState::Walk;
				paradinAnimationT_ = 0.0f;
				break;
			}
			if (attackButton) {
				curParadinState_ = ParadinState::AtkSword;
				paradinAnimationT_ = 0.0f;
				break;
			}

			MAGISYSTEM::ApplyAnimationSkinModel("Paradin", MAGISYSTEM::FindAnimation("Paradin_Idle"), paradinAnimationT_, true);
			paradinAnimationT_ += MAGISYSTEM::GetDeltaTime();
		}
		break;
		case ParadinState::Walk:
		{
			if (!Length(ls)) {
				curParadinState_ = ParadinState::Idle;
				paradinAnimationT_ = 0.0f;
				break;
			}
			if (attackButton) {
				curParadinState_ = ParadinState::AtkSword;
				paradinAnimationT_ = 0.0f;
				break;
			}
			// 移動方向をカメラの方向に向ける
			if (auto cucam = MAGISYSTEM::GetCurrentCamera3D()) {
				Vector3 forward = cucam->GetTarget() - cucam->GetEye();
				forward.y = 0.0f;
				forward = Normalize(forward);
				Vector3 right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));

				// 移動方向決定
				const Vector3 dir = Normalize(right * ls.x + forward * ls.y);

				// 移動量決定
				velocity = dir * paradinSpeed_ * MAGISYSTEM::GetDeltaTime();

				// 移動方向にキャラを向ける
				float yaw = std::atan2(dir.x, dir.z);
				const Quaternion dirQ = MakeRotateAxisAngleQuaternion({ 0.0f,1.0f,0.0f }, yaw);
				paradinTrans_->SetQuaternion(dirQ);
			}

			MAGISYSTEM::ApplyAnimationSkinModel("Paradin", MAGISYSTEM::FindAnimation("Paradin_Walking"), paradinAnimationT_, true);
			paradinAnimationT_ += MAGISYSTEM::GetDeltaTime();
		}
		break;
		case ParadinState::AtkSword:
		{
			const float attackCancelTime = 1.0f;
			const float aniSpdMul = 1.5f;

			if (paradinAnimationT_ > attackCancelTime) {
				if (attackButton) {
					curParadinState_ = ParadinState::AtkKick;
					paradinAnimationT_ = 0.0f;
					break;
				}
			}

			bool playAnimation = MAGISYSTEM::ApplyAnimationSkinModel("Paradin", MAGISYSTEM::FindAnimation("Paradin_Attack_Sword_0"), paradinAnimationT_, false);
			paradinAnimationT_ += MAGISYSTEM::GetDeltaTime() * aniSpdMul;

			if (!playAnimation) {
				curParadinState_ = ParadinState::Idle;
				paradinAnimationT_ = 0.0f;
				break;
			}
		}
		break;
		case ParadinState::AtkKick:
		{
			const float aniSpdMul = 1.2f;

			bool playAnimation = MAGISYSTEM::ApplyAnimationSkinModel("Paradin", MAGISYSTEM::FindAnimation("Paradin_Attack_Kick_0"), paradinAnimationT_, false);
			paradinAnimationT_ += MAGISYSTEM::GetDeltaTime() * aniSpdMul;

			if (!playAnimation) {
				curParadinState_ = ParadinState::Idle;
				paradinAnimationT_ = 0.0f;
				break;
			}
			break;
		}

	}

	paradinTrans_->AddTranslate(velocity);

	// 踊ってるやつら
	brainStemT_ += MAGISYSTEM::GetDeltaTime();
	MAGISYSTEM::ApplyAnimationSkinModel("BrainStem", MAGISYSTEM::FindAnimation("BrainStem"), brainStemT_, true);

}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	for (uint32_t i = 0; i < brainStemNum_; i++) {
		MAGISYSTEM::DrawSkinModel("BrainStem", brainStemTrans_[i]->GetWorldMatrix(), ModelMaterial{});
	}

	MAGISYSTEM::DrawSkinModel("Paradin", paradinTrans_->GetWorldMatrix(), ModelMaterial{});
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
