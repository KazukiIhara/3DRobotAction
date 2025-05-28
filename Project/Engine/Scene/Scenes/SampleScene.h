#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

#include "Transform3D/Transform3D.h"

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
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;
	std::unique_ptr<Camera2D> sceneCamera2D_ = nullptr;

	// トランスフォーム
	std::unique_ptr<Transform3D> transform_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_[5]{};

	// 板ポリ描画用の頂点データ
	PlaneData3D planeData_{};

	// 三角形描画用の頂点データ
	TriangleData3D triangleData_{};

	// ボックス描画用の頂点データ
	BoxData3D boxData_{};

	// 球体描画用の頂点データ
	SphereData3D sphereData_{};

	// リング描画用の頂点データ
	RingData3D ringData_{};

	// シリンダー描画用の頂点データ
	CylinderData3D cylinderData_{};

	// プリミティブ描画用のマテリアルデータ
	PrimitiveMaterialData3D material_{};

	// スプライト用のマテリアルデータ
	SpriteMaterialData spriteMaterial{};

	// モデル用のマテリアルデータ
	ModelMaterial modelMaterial_{};

	// モデル用のマテリアルデータ
	ModelMaterial modelMatAlpha_{};

	float vignetteScale_ = 16.0f;
	float vignetteFalloff_ = 0.8f;

	float gaussianSigma_ = 0.5f;

	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;

	bool isOnGrayscale_ = false;
	bool isOnGaussian_ = false;
	bool isOnVignette_ = false;
	bool isRadialBlur_ = false;

	static const uint32_t wtsNum_ = 3;

	std::array<WorldTransform, wtsNum_> wts_;


	// DirectionalLight
	DirectionalLight directionalLight_{};

	// スプライトデータ
	SpriteData spriteData_{};
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
	sceneCamera_ = std::make_unique<Camera3D>("SceneCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera");


	// 2Dカメラ作成
	sceneCamera2D_ = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");


	// ライト
	MAGISYSTEM::AddPunctualLight("SampleLight");

	// スカイボックスの設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	// モデルのマテリアル設定
	modelMaterial_.blendMode = BlendMode::None;
	modelMatAlpha_.blendMode = BlendMode::Add;

	// スプライト用のマテリアルデータ
	spriteMaterial.blendmode = BlendMode::Normal;
	spriteMaterial.color = Vector4(1.0f, 1.0f, 1.0f, 0.5f);
	spriteMaterial.textureName = "pronama_chan.png";
	spriteMaterial.isFlipX = false;

	// ModelDrawer
	MAGISYSTEM::CreateModelDrawer("test", MAGISYSTEM::FindModel("teapot"));

	// トランスフォーム初期化
	for (uint32_t i = 0; i < 5; i++) {
		worldTransform_[i].Initialize();
	}

	worldTransform_[3].translate_.x = 2.0f;
	worldTransform_[4].translate_.x = 4.0f;

	// マテリアルを設定
	material_.blendMode = BlendMode::None;

	for (uint32_t i = 0; i < wtsNum_; i++) {
		wts_[i].Initialize();
		wts_[i].translate_.x = -6.0f;
		wts_[i].translate_.z = float(i) * 2.0f;
		wts_[i].translate_.y = 2.0f;
	}

	worldTransform_[0].translate_.x = 0.0f;
	worldTransform_[0].rotate_.x = std::numbers::pi_v<float> *0.5f;
	planeData_.verticesOffsets[Plane3DVertices::LeftTop] = { -10.0f,10.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightTop] = { 10.0f,10.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::LeftBottom] = { -10.0f,-10.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightBottom] = { 10.0f,-10.0f,0.0f };

	worldTransform_[1].translate_.x = 1.5f;
	worldTransform_[1].translate_.y = 1.0f;

	worldTransform_[2].translate_.x = -2.0f;
	worldTransform_[2].translate_.y = 1.0f;


	transform_ = std::make_unique<Transform3D>();
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("NewTranslate");
	ImGui::DragFloat3("Scale", &transform_->GetScale().x, 0.01f);
	ImGui::DragFloat3("Rotate", &transform_->GetRotate().x, 0.01f);
	ImGui::DragFloat3("Translate", &transform_->GetTranslate().x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate0");
	ImGui::DragFloat3("Scale", &worldTransform_[0].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[0].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[0].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate1");
	ImGui::DragFloat3("Scale", &worldTransform_[1].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[1].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[1].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate2");
	ImGui::DragFloat3("Scale", &worldTransform_[2].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[2].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[2].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate3");
	ImGui::DragFloat3("Scale", &worldTransform_[3].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[3].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[3].translate_.x, 0.01f);
	ImGui::End();


	ImGui::Begin("Translate4");
	ImGui::DragFloat3("Scale", &worldTransform_[4].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[4].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[4].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("PlaneData");
	ImGui::DragFloat3("LeftTop", &planeData_.verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTop", &planeData_.verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottom", &planeData_.verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottom", &planeData_.verticesOffsets[3].x, 0.01f);
	ImGui::End();

	ImGui::Begin("BoxData");
	ImGui::DragFloat3("LeftTopFront", &boxData_.verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTopFront", &boxData_.verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottomFront", &boxData_.verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottomFront", &boxData_.verticesOffsets[3].x, 0.01f);
	ImGui::DragFloat3("LeftTopBack", &boxData_.verticesOffsets[4].x, 0.01f);
	ImGui::DragFloat3("RightTopBack", &boxData_.verticesOffsets[5].x, 0.01f);
	ImGui::DragFloat3("LeftBottomBack", &boxData_.verticesOffsets[6].x, 0.01f);
	ImGui::DragFloat3("RightBottomBack", &boxData_.verticesOffsets[7].x, 0.01f);
	ImGui::End();

	ImGui::Begin("SphereData");
	int tempH = sphereData_.horizontalSegments;
	ImGui::DragInt("HorizontalSegments", &tempH);
	sphereData_.horizontalSegments = tempH;
	int tempV = sphereData_.verticalSegments;
	ImGui::DragInt("VerticalSegments", &tempV);
	sphereData_.verticalSegments = tempV;
	ImGui::DragFloat("Radius", &sphereData_.radius);
	ImGui::End();

	ImGui::Begin("RingData");
	int ringDivideTemp = ringData_.ringDivide;
	ImGui::DragInt("Divide", &ringDivideTemp);
	ringData_.ringDivide = ringDivideTemp;
	ImGui::DragFloat("InnerRadius", &ringData_.innerRadius, 0.01f);
	ImGui::DragFloat("OuterRadius", &ringData_.outerRadius, 0.01f);
	ImGui::End();

	ImGui::Begin("CylinderData");
	int cylinderDivideTemp = cylinderData_.divide;
	ImGui::DragInt("Divide", &cylinderDivideTemp);
	cylinderData_.divide = cylinderDivideTemp;
	ImGui::DragFloat("TopRadius", &cylinderData_.topRadius, 0.01f);
	ImGui::DragFloat("BottomRadius", &cylinderData_.bottomRadius, 0.01f);
	ImGui::DragFloat("Height", &cylinderData_.height, 0.01f);
	ImGui::End();

	ImGui::Begin("Material");
	ImGui::DragFloat2("uvScale", &material_.uvScale.x, 0.01f);
	ImGui::DragFloat("uvRotate", &material_.uvRotate, 0.01f);
	ImGui::DragFloat2("uvTranslate", &material_.uvTranslate.x, 0.01f);
	ImGui::ColorEdit4("Color", &material_.baseColor.x);
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

	ImGui::Begin("Sound");
	if (ImGui::Button("Coin")) {
		MAGISYSTEM::PlayWaveSound("coin.wav");
	}
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	ImGui::DragFloat3("Direction", &directionalLight_.direction.x, 0.01f);
	directionalLight_.direction = MAGIMath::Normalize(directionalLight_.direction);
	ImGui::DragFloat("Intensity", &directionalLight_.intensity, 0.01f);
	ImGui::ColorEdit3("Color", &directionalLight_.color.x);
	ImGui::End();

	ImGui::Begin("SpriteData");
	ImGui::DragFloat2("Position", &spriteData_.position.x, 1.0f);
	ImGui::Checkbox("IsFlipX", &spriteMaterial.isFlipX);
	ImGui::Checkbox("IsFlipY", &spriteMaterial.isFlipY);
	ImGui::End();

	// トランスフォーム更新
	for (uint32_t i = 0; i < 5; i++) {
		worldTransform_[i].Update();
	}

	for (uint32_t i = 0; i < wtsNum_; i++) {
		wts_[i].Update();
	}

	transform_->Update();

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

	// スプライト描画
	//MAGISYSTEM::DrawSprite(SpriteData{}, SpriteMaterialData{});
	//MAGISYSTEM::DrawSprite(SpriteData{}, spriteMaterial);

	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(worldTransform_[0].worldMatrix_, planeData_, material_);

	// 球体描画
	MAGISYSTEM::DrawSphere3D(transform_->GetWorldMatrix(), sphereData_, material_);

	// ボックス描画
	MAGISYSTEM::DrawBox3D(worldTransform_[2].worldMatrix_, boxData_, material_);

	// 三角形描画
	//MAGISYSTEM::DrawTriangle3D(worldTransform_[2].worldMatrix_, triangleData_, material_);

	// リング描画
	// MAGISYSTEM::DrawRing3D(worldTransform_[3].worldMatrix_, ringData_, material_);

	// シリンダー描画
	MAGISYSTEM::DrawCylinder3D(worldTransform_[4].worldMatrix_, cylinderData_, material_);

	// モデル描画
	// MAGISYSTEM::DrawModel("test", worldTransform_[2].worldMatrix_, modelMatAlpha_);

	// モデル描画


	for (uint32_t i = 0; i < wtsNum_; i++) {
		MAGISYSTEM::DrawModel("test", wts_[i].worldMatrix_, modelMaterial_);
	}

	//MAGISYSTEM::DrawLine3D(Vector3(0.0f, -2.0f, 0.0f), Vector3(0.0f, 2.0f, 0.0f), Color::Crimson);



	//for (uint32_t i = 0; i < wtsNum_; i++) {
	//	MAGISYSTEM::DrawSphere3D(wts_[i].worldMatrix_, sphereData_, material_);
	//}


}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
