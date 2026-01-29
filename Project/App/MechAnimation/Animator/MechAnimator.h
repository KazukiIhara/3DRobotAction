#pragma once

// C++
#include <array>
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

	// 指定ジョイントのアニメ適用を切り替え
	void SetJointAnimationEnabled(MechAnimation::TransType joint, bool enabled);

	// 脚ジョイント一括のアニメ適用を切り替え
	void SetLegAnimationEnabled(bool enabled);

	// 接地時に脚アニメを自動無効化するか
	void SetAutoDisableLegOnGround(bool enabled);

	// Waist回転アニメ適用を切り替え
	void SetWaistRotationAnimationEnabled(bool enabled);
	// Waist回転アニメ適用が有効か
	bool IsWaistRotationAnimationEnabled() const;

	// 頭回転アニメ適用を切り替え
	void SetHeadRotationAnimationEnabled(bool enabled);
	// 頭回転アニメ適用が有効か
	bool IsHeadRotationAnimationEnabled() const;

	// 体回転アニメ適用を切り替え
	void SetBodyRotationAnimationEnabled(bool enabled);
	// 体回転アニメ適用が有効か
	bool IsBodyRotationAnimationEnabled() const;

	// 左腕回転アニメ適用を切り替え
	void SetLeftArmRotationAnimationEnabled(bool enabled);
	// 左腕回転アニメ適用が有効か
	bool IsLeftArmRotationAnimationEnabled() const;

	// 右腕回転アニメ適用を切り替え
	void SetRightArmRotationAnimationEnabled(bool enabled);
	// 右腕回転アニメ適用が有効か
	bool IsRightArmRotationAnimationEnabled() const;

private:
	MechAnimation::Pose CaptureCurrentPose() const;
	void ApplyPose(const MechAnimation::Pose& pose);
	MechAnimation::Pose SampleClipPose(const MechAnimation::Clip& clip, float t) const;

	// 指定ジョイントがアニメ適用対象か
	bool IsJointAnimationEnabled(MechAnimation::TransType joint) const;

private:
	MechAnimationContainer* container_ = nullptr;
	BaseMech* mech_ = nullptr;

	// ジョイントごとのアニメ適用フラグ
	std::array<bool, MechAnimation::kJointCount> jointAnimEnabled_{};

	// Waist回転だけアニメ適用を切る
	bool isWaistRotAnimEnabled_ = true;

	// 頭回転だけアニメ適用を切る
	bool isHeadRotAnimEnabled_ = true;

	// 体回転だけアニメ適用を切る
	bool isBodyRotAnimEnabled_ = true;

	// 左腕回転だけアニメ適用を切る
	bool isLeftArmRotAnimEnabled_ = true;

	// 右腕回転だけアニメ適用を切る
	bool isRightArmRotAnimEnabled_ = true;

	// 接地時に脚アニメを止める
	bool autoDisableLegOnGround_ = true;

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
