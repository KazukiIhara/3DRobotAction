#pragma once

#include <memory>
#include <array>

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"
#include "GameObject/Enemy/Enemy.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
/// <typeparam name="Data"></typeparam>
template <typename Data>
class PlayScene:public BaseScene<Data> {
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

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// ポストエフェクトの用の変数
	float vignetteScale_ = 18.0f;
	float vignetteFalloff_ = 0.8f;
	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;
	float gaussianSigma_ = 0.5f;

	// 
	// デバッグ用
	// 


	PlaneEffectParam planeEffect_;
};

template<typename Data>
inline void PlayScene<Data>::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

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

	MAGISYSTEM::LoadModel("StageObj0");
	MAGISYSTEM::CreateModelDrawer("StageObj0", MAGISYSTEM::FindModel("StageObj0"));

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));

	MAGISYSTEM::LoadModel("MechHead");
	MAGISYSTEM::CreateModelDrawer("MechHead", MAGISYSTEM::FindModel("MechHead"));

	MAGISYSTEM::LoadModel("MechBody");
	MAGISYSTEM::CreateModelDrawer("MechBody", MAGISYSTEM::FindModel("MechBody"));

	MAGISYSTEM::LoadModel("MechRightArm");
	MAGISYSTEM::CreateModelDrawer("MechRightArm", MAGISYSTEM::FindModel("MechRightArm"));

	MAGISYSTEM::LoadModel("MechLeftArm");
	MAGISYSTEM::CreateModelDrawer("MechLeftArm", MAGISYSTEM::FindModel("MechLeftArm"));

	MAGISYSTEM::LoadModel("MechLeg");
	MAGISYSTEM::CreateModelDrawer("MechLeg", MAGISYSTEM::FindModel("MechLeg"));


	//-------------------------------------------------------
	// シーン固有の初期化処理
	//-------------------------------------------------------

	// スカイボックスを設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	// プレイヤー作成
	player_ = std::make_unique<Player>();

	// 敵作成
	enemy_ = std::make_unique<Enemy>();


	// プレイヤーのターゲット対象に敵を追加
	player_->GetMechCore().lock()->GetLockOnComponent()->AddMech(enemy_->GetMechCore());


	planeEffect_.Initialize();
	planeEffect_.blendMode = BlendMode::Normal;
	planeEffect_.translate.isAnimated = true;
	planeEffect_.translate.startTime = 0.0f;
	planeEffect_.translate.animation.SetEasing(EasingType::EaseInCubic);
	planeEffect_.translate.animation.SetEnd(Vector3(0.0f, 0.5f, 0.0f));
	planeEffect_.color.isAnimated = true;
	planeEffect_.color.animation.SetEnd(Vector4(1.0f, 1.0f, 1.0f, 0.0f));

}

template<typename Data>
inline void PlayScene<Data>::Update() {

	ImGui::Begin("Scene");
	if (ImGui::Button("SceneImport")) {
		MAGISYSTEM::LoadSceneDataFromJson("SceneData");
		MAGISYSTEM::ImportSceneData("SceneData", true);
		if (auto cameraObj = MAGISYSTEM::FindGameObject3D("Camera").lock()) {
			if (auto camera = cameraObj->GetCamera3D("Camera").lock()) {
				camera->SetTarget(Vector3(0.0f, 0.0f, 0.0f));
				camera->ApplyCurrent();
			}
		}
	}

	if (ImGui::Button("PlayCameraAnimation")) {
		if (auto cameraObj = MAGISYSTEM::FindGameObject3D("Camera").lock()) {
			if (auto camera = cameraObj->GetCamera3D("Camera").lock()) {
				camera->StartEyeAnimation();
			}
		}
	}
	ImGui::End();

	ImGui::Begin("AddEffect");
	if (ImGui::Button("Plane")) {
		MAGISYSTEM::AddPlaneEffect(planeEffect_);
	}
	ImGui::End();

	/*ImGui::Begin("VignetteParamater");
	ImGui::DragFloat("Scale", &vignetteScale_, 0.01f);
	ImGui::DragFloat("Falloff", &vignetteFalloff_, 0.01f);
	ImGui::End();

	ImGui::Begin("GaussianBlurParamater");
	ImGui::DragFloat("Sigma", &gaussianSigma_, 0.01f);
	ImGui::End();*/

	// ライト変数
	MAGISYSTEM::SetDirectionalLight(directionalLight_);


	// プレイヤー更新
	player_->Update();

	// 敵更新
	enemy_->Update();

	// ポストエフェクト適用
	MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
}

template<typename Data>
inline void PlayScene<Data>::Draw() {

	// プレイヤーにまつわるもの描画
	player_->Draw();


}

template<typename Data>
inline void PlayScene<Data>::Finalize() {

}
