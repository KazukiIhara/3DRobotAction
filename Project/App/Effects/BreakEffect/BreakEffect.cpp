#include "BreakEffect.h"

using namespace MAGIMath;
using namespace MAGIUtility;

BreakEffect::BreakEffect(Vector3 worldPosition) {

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
	ringMaterial_.blendMode = BlendMode::Normal;
	ringMaterial_.textureName = "gradation.png";
	ringMaterial_.baseColor = { 1.0f,0.5f,0.0f,1.0f };

	ringRotates_[0] = { 0.7f,1.2f,-0.43f };
	ringRotates_[1] = { 1.11f,0.0f,0.0f };
	ringRotates_[2] = { 0.0f,1.4f,-0.9f };
	ringRotates_[3] = { 0.0f,-1.17f,0.92f };


	ringOuterAnimation_[0] = std::make_unique<SimpleAnimation<float>>(1.0f, 5.0f, EasingType::EaseOutQuart);
	ringInnerAnimation_[0] = std::make_unique<SimpleAnimation<float>>(0.8f, 4.0f, EasingType::EaseOutQuart);

	ringOuterAnimation_[1] = std::make_unique<SimpleAnimation<float>>(1.0f, 7.0f, EasingType::EaseOutQuart);
	ringInnerAnimation_[1] = std::make_unique<SimpleAnimation<float>>(0.8f, 6.0f, EasingType::EaseOutQuart);

	//===================================
	// 粒子のデータを初期化
	//===================================
	starEmitter_ = MAGISYSTEM::FindEmitter3D("Star");
	starParticle_ = MAGISYSTEM::FindParticleGroup3D("Star");
	starEmitter_->worldPosition = corePosition_;

	starEmitter_->AddParticleGroup(starParticle_);

	starSetting_.emitType = EmitType::Random;
	starSetting_.minColor = Color::SkyBlue;
	starSetting_.maxColor = Color::SkyBlue;

	starSetting_.minLifeTime = 2.5f;
	starSetting_.maxLifeTime = 3.0f;
	starSetting_.count = 600;

	starSetting_.minVelocity = { -1.5f,-1.5f,-1.5f };
	starSetting_.maxVelocity = { 1.5f,1.5f,1.5f };

	starEmitter_->GetEmitterSetting() = starSetting_;
	starParticle_->GetBlendMode() = BlendMode::Normal;

}

BreakEffect::~BreakEffect() {

}

void BreakEffect::SetRingRotates(std::array<Vector3, 4> rotates) {
	ringRotates_ = rotates;
}

void BreakEffect::SetRingColor(Vector4 color) {
	ringMaterial_.baseColor = color;
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
	case State::Finish:
		UpdateFinish();
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
	case State::Finish:
		DrawFinish();
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
		starEmitter_->EmitAll();
	}
}

void BreakEffect::UpdateExplosion() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	// リングの処理
	for (uint32_t i = 0; i < 2; i++) {
		ringDatas_[i].outerRadius = ringOuterAnimation_[0]->GetValue(timer_ / explosionTime_);
		ringDatas_[i].innerRadius = ringInnerAnimation_[0]->GetValue(timer_ / explosionTime_);
	}
	for (uint32_t i = 2; i < 4; i++) {
		ringDatas_[i].outerRadius = ringOuterAnimation_[1]->GetValue(timer_ / explosionTime_);
		ringDatas_[i].innerRadius = ringInnerAnimation_[1]->GetValue(timer_ / explosionTime_);
	}

	// パーティクルの処理

	if (timer_ >= explosionTime_) {
		timer_ = 0.0f;
		currentState_ = State::Finish;
	}
}

void BreakEffect::UpdateFinish() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	ringMaterial_.baseColor.w = 1.0f - timer_ / finishTime_;

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

void BreakEffect::DrawFinish() {
	for (uint32_t i = 0; i < 4; i++) {
		Matrix4x4 wMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, ringRotates_[i], corePosition_);
		MAGISYSTEM::DrawRing3D(wMat, ringDatas_[i], ringMaterial_);
	}
}
