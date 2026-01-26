#define NOMINMAX

#include "FadeSystem.h"

// C++
#include <algorithm>

#include "MAGI.h"

using namespace Magi;

FadeSystem::FadeSystem() {
	MAGISYSTEM::LoadTexture("white.png");
	data_.size = { WindowApp::kClientWidth,WindowApp::kClientHeight };
	mat_.textureName = "white.png";

	mat_.color = Color::Black;
}

void FadeSystem::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();
	// 状態なし
	if (state_ == State::None) {
		return;
	}

	// 時間更新
	timeSec_ += dt;

	// 進行度算出
	float t = 0.0f;
	if (durationSec_ > 0.0f) {
		t = timeSec_ / durationSec_;
	}
	t = std::clamp(t, 0.0f, 1.0f);

	// アルファ更新
	if (state_ == State::FadeOut) {
		alpha_ = t;
	}
	if (state_ == State::FadeIn) {
		alpha_ = 1.0f - t;
	}

	// 完了処理
	if (t >= 1.0f) {
		if (state_ == State::FadeOut) {
			alpha_ = 1.0f;
		}
		if (state_ == State::FadeIn) {
			alpha_ = 0.0f;
			state_ = State::None;
		}
	}
}

void FadeSystem::Draw() {
	// 描画不要
	if (alpha_ <= 0.0f) {
		return;
	}

	mat_.color.w = alpha_;

	MAGISYSTEM::DrawSprite(data_, mat_);
}

void FadeSystem::StartFadeOut(float durationSec) {
	// フェードアウト開始
	state_ = State::FadeOut;

	// 進行初期化
	durationSec_ = std::max(0.0001f, durationSec);
	timeSec_ = 0.0f;

	// 初期アルファ
	alpha_ = 0.0f;
}

void FadeSystem::StartFadeIn(float durationSec) {
	// フェードイン開始
	state_ = State::FadeIn;

	// 進行初期化
	durationSec_ = std::max(0.0001f, durationSec);
	timeSec_ = 0.0f;

	// 初期アルファ
	alpha_ = 1.0f;
}

bool FadeSystem::IsBusy() const {
	// 実行中判定
	return state_ != State::None;
}

bool FadeSystem::IsFadeOutFinished() const {
	// フェードアウト完了判定
	if (state_ != State::FadeOut) {
		return false;
	}
	return alpha_ >= 1.0f;
}

void FadeSystem::SetBlack() {
	// 黒固定
	state_ = State::None;
	durationSec_ = 0.0f;
	timeSec_ = 0.0f;
	alpha_ = 1.0f;
}

void FadeSystem::SetClear() {
	// 透明固定
	state_ = State::None;
	durationSec_ = 0.0f;
	timeSec_ = 0.0f;
	alpha_ = 0.0f;
}
