#pragma once

// C++
#include <string>

#include "MechAnimation/MechAnimation.h"

// Forward
class BaseMech;
class Transform3D;
class MechAnimationContainer;

/// <summary>
/// 機体アニメーション再生クラス
/// </summary>
class MechAnimator {
public:
	MechAnimator(MechAnimationContainer* container, BaseMech* mech);
	~MechAnimator() = default;

	// アニメーション適用
	void ApplyAnimation(const std::string& name, float t, float blendT = 0.0f);

private:
	// 現在姿勢を保存
	MechAnimation::Pose CaptureCurrentPose() const;

	// Poseを適用
	void ApplyPose(const MechAnimation::Pose& pose);

	// クリップ内補間Poseを取得
	MechAnimation::Pose SampleClipPose(const MechAnimation::Clip& clip, float t) const;

private:
	MechAnimationContainer* container_ = nullptr;
	BaseMech* mech_ = nullptr;

	// 補間用の開始姿勢
	MechAnimation::Pose blendFromPose_{};

	// 再生中クリップ名
	std::string playingClipName_;
};
