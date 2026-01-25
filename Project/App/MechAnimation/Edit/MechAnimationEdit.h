#pragma once

#include <string>

#include "MechAnimation/MechAnimation.h"
#include "Includes/Easing/Easing.h"

class BaseMech;
class MechAnimationContainer;

/// <summary>
/// 機体アニメーション作成クラス
/// </summary>
class MechAnimationEdit {
public:
	MechAnimationEdit(MechAnimationContainer* container);
	~MechAnimationEdit() = default;

	void Update();

	void SetBaseMech(BaseMech* mech);

	bool AddAnimationClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite);

private:
	void ShowWindow();

	void DrawRotate(BaseMech* mech, int typeValue, const char* label);
	void DrawTranslate(BaseMech* mech, int typeValue, const char* label);

	// 現在の姿勢をPoseとして取得
	MechAnimation::Pose CaptureCurrentPose() const;

	// 選択中ClipにPoseを追加
	void AddPoseToSelectedClip();

	// 選択中Poseを削除
	void RemoveSelectedPose();

	// 現在の姿勢を選択中のPoseに適用
	void ApplyCurrentToSelectedPose();

	// 現在の姿勢を適用
	void ApplyPoseToMech(const MechAnimation::Pose& pose);

	// 右にPose一覧表示
	void ShowPoseList();

	// 再生更新
	void UpdatePlayback();

	// t適用
	void ApplyAtNormalizedTime(float t);

private:
	BaseMech* mech_ = nullptr;
	MechAnimationContainer* container_ = nullptr;

	// 追加用入力
	char clipName_[128] = "NewClip";
	bool overwrite_ = false;

	// クリップ選択
	int selectedClipIndex_ = -1;
	std::string selectedClipName_;

	// Pose選択
	int selectedPoseIndex_ = -1;

	// 再生状態
	bool isPlaying_ = false;

	// 再生時間
	float playDurationSec_ = 1.0f;
	float playTimeSec_ = 0.0f;

	// ブレンド時間
	float lerpTimeSec_ = 0.0f;

	// イージング
	EasingType easingType_ = EasingType::Linear;
};
