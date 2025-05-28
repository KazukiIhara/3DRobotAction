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

	//===================================
	// 靄のデータを初期化
	//===================================


	//===================================
	// リングのデータを初期化
	//===================================
	for (size_t i = 0; i < 4; i++) {
		ringDatas_[i] = RingData3D{};
		ringRotates_[i] = { 0.0f,0.0f,0.0f };
	}
	ringMaterial_.blendMode = BlendMode::Add;
	ringMaterial_.textureName = "gradation.png";


	//===================================
	// 粒子のデータを初期化
	//===================================


}

BreakEffect::~BreakEffect() {

}

void BreakEffect::Update() {
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
	Matrix4x4 wMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, ringRotates_[0], corePosition_);
	MAGISYSTEM::DrawRing3D(wMat, ringDatas_[0], ringMaterial_);
}

bool BreakEffect::IsFinished() {
	return false;
}

void BreakEffect::UpdateElectric() {

}

void BreakEffect::UpdateHaze() {

}

void BreakEffect::UpdateExplosion() {

}