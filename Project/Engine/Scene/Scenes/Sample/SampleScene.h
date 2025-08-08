#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "SimpleAnimation/SimpleAnimation.h"

#include "3D/Cameras3D/ThirdPersonCamera/ThirdPersonCamera.h"

#include "Random/Random.h"

using namespace MAGIUtility;

enum class ParadinState {
	Idle,
	Walk,
	AtkSword,
	AtkKick,
};

// サンプルシーン
class SampleScene : public BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
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
	std::array<Transform3D*, brainStemNum_> brainStemTrans_;
	float brainStemT_ = 0.0f;

	// texIndex
	uint32_t circleIndex_;



	float debugTimer_ = 0.0f;

};