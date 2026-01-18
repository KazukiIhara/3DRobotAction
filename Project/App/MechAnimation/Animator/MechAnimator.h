#pragma once

// C++
#include <string>

// 前方宣言
class BossMech;
class Transform3D;
class MechAnimationContainer;

#include "MechAnimation/MechAnimation.h"

/// <summary>
/// 機体アニメーション再生クラス
/// </summary>
class MechAnimator {
public:
	MechAnimator() = default;
	MechAnimator(MechAnimationContainer* container, BossMech* mech);
	~MechAnimator() = default;

	// 参照を設定
	void SetContainer(MechAnimationContainer* container);
	void SetBossMech(BossMech* mech);

	// 再生対象クリップ名を設定
	bool SetClip(const std::string& name);

	// 再生対象クリップ名を取得
	const std::string& GetClipName() const;

	// t(0-1) で姿勢を適用
	void ApplyAnimation(float t);

private:
	// 現在クリップを取得
	const MechAnimation::Clip* GetCurrentClip() const;

	// Poseを機体へ適用
	void ApplyPose(const MechAnimation::Pose& pose);

	// Pose同士を補間して機体へ適用
	void ApplyPoseLerp(const MechAnimation::Pose& a, const MechAnimation::Pose& b, float t);

private:
	MechAnimationContainer* container_ = nullptr;
	BossMech* mech_ = nullptr;

	std::string clipName_;
};
