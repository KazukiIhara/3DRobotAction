#include "BreakEffect.h"

using namespace MAGIMath;
using namespace MAGIUtility;

BreakEffect::BreakEffect(Vector3 worldPosition) {
	//===================================
	// テクスチャのロード
	//===================================

	// 靄用
	MAGISYSTEM::LoadTexture("smoke.png");

	// リング用
	MAGISYSTEM::LoadTexture("gradation.png");

	// 粒子用
	MAGISYSTEM::LoadTexture("star.png");

	//===================================
	// コアの設定
	//===================================

	corePosition_ = worldPosition;
	currentState_ = State::Electric;
	timer_ = 0.0f;

	//===================================
	// 靄のデータを初期化
	//===================================




	//===================================
	// リングのデータを初期化
	//===================================
	for (size_t i = 0; i < 4; i++) {
		ringDatas_[i] = RingData3D{};
	}
	ringMaterial_.blendMode = BlendMode::Add;
	ringMaterial_.textureName = "gradation.png";

	ringRotates_[0] = { 0.0f,0.5f,0.0f };
	ringRotates_[1] = { 0.0f,-0.5f,0.0f };
	ringRotates_[2] = { 0.0f,1.0f,0.0f };
	ringRotates_[3] = { 0.0f,-1.0f,0.0f };


	//===================================
	// 粒子のデータを初期化
	//===================================


}

BreakEffect::~BreakEffect() {

}

void BreakEffect::Update() {
	ImGui::Begin("EffectParamater");

	ImGui::DragFloat3("RingRotate0", &ringRotates_[0].x, 0.01f);
	ImGui::DragFloat3("RingRotate1", &ringRotates_[1].x, 0.01f);
	ImGui::DragFloat3("RingRotate2", &ringRotates_[2].x, 0.01f);
	ImGui::DragFloat3("RingRotate3", &ringRotates_[3].x, 0.01f);

	ImGui::End();

	switch (currentState_) {
	case State::Electric:
		UpdateElectric();
		break;
	case State::Haze:
		UpdateHaze();
		break;
	case State::Explosion:
		UpdateExplosion();
		break;
	}
}

void BreakEffect::Draw() {
	switch (currentState_) {
	case State::Electric:
		DrawElectric();
		break;
	case State::Haze:
		DrawHaze();
		break;
	case State::Explosion:
		DrawExplosion();
		break;
	}
}

bool BreakEffect::IsFinished() {
	return isFinished_;
}

void BreakEffect::UpdateElectric() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	if (timer_ >= electricTime_) {
		timer_ = 0.0f;
		currentState_ = State::Haze;
	}
}

void BreakEffect::UpdateHaze() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	if (timer_ >= electricTime_) {
		timer_ = 0.0f;
		currentState_ = State::Explosion;
	}
}

void BreakEffect::UpdateExplosion() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	const float scalingSize = 5.0f;

	for (uint32_t i = 0; i < 4; i++) {
		ringDatas_[i].outerRadius += scalingSize * MAGISYSTEM::GetDeltaTime();
		ringDatas_[i].innerRadius += scalingSize * MAGISYSTEM::GetDeltaTime();
	}

	if (timer_ >= explosionTime_) {
		timer_ = 0.0f;
		isFinished_ = true;
	}
}

void BreakEffect::DrawElectric() {

}

void BreakEffect::DrawHaze() {

}

void BreakEffect::DrawExplosion() {
	for (uint32_t i = 0; i < 4; i++) {
		Matrix4x4 wMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, ringRotates_[i], corePosition_);
		MAGISYSTEM::DrawRing3D(wMat, ringDatas_[i], ringMaterial_);
	}
}
