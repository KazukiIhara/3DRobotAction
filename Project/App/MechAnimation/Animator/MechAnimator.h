#pragma once

// C++
#include <string>

// Forward
class MechAnimationContainer;
class BaseMech;

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "Math/Utility/MathUtility.h"

// easing
#include "Includes/Easing/Easing.h"

/// <summary>
/// 機体アニメーションクラス
/// </summary>
class MechAnimator {
public:
	MechAnimator(MechAnimationContainer* container, BaseMech* mech);
	~MechAnimator() = default;

	// name と t からその時点の姿勢を適用
	void ApplyAnimation(const std::string& name, float t);

	// アニメーション再生開始
	void PlayAnimation(
		const std::string& name,
		float durationSec,
		float blendSec,
		EasingType easing = EasingType::Linear,
		MechAnimation::LoopType loopType = MechAnimation::LoopType::None
	);
	// 再生停止
	void StopAnimation();

	// 再生更新
	void Update();

	// 再生中か
	bool IsPlaying() const {
		return isPlaying_;
	}

private:
	MechAnimation::Pose CaptureCurrentPose() const;
	void ApplyPose(const MechAnimation::Pose& pose);
	MechAnimation::Pose SampleClipPose(const MechAnimation::Clip& clip, float t) const;

private:
	MechAnimationContainer* container_ = nullptr;
	BaseMech* mech_ = nullptr;

	// 再生状態
	bool isPlaying_ = false;
	float playTimeSec_ = 0.0f;
	float durationSec_ = 0.0f;
	float blendSec_ = 0.0f;
	EasingType easing_ = EasingType::Linear;

	std::string playingClipName_;

	// ブレンド開始姿勢
	MechAnimation::Pose blendFromPose_{};

	MechAnimation::LoopType loopType_ = MechAnimation::LoopType::None;
	bool pingPongForward_ = true;

};
