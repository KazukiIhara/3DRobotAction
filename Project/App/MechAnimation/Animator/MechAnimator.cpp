#define NOMINMAX

#include "MechAnimator.h"

// C++
#include <algorithm>

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "GameObject/Boss/Mech/BossMech.h"
#include "3D/Transform3D/Transform3D.h"

// 数学
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

MechAnimator::MechAnimator(MechAnimationContainer* container, BossMech* mech) {
	container_ = container;
	mech_ = mech;
}

void MechAnimator::ApplyAnimation(const std::string& name, float t, float blendT) {

	const MechAnimation::Clip* clip = container_->GetClip(name);
	if (!clip) {
		return;
	}

	if (clip->frames.empty()) {
		return;
	}

	// 範囲クランプ
	t = std::clamp(t, 0.0f, 1.0f);

	// 再生開始時に現在姿勢を保存
	if (t == 0.0f) {
		playingClipName_ = name;
		blendFromPose_ = CaptureCurrentPose();
	}

	// クリップをサンプル
	const MechAnimation::Pose targetPose = SampleClipPose(*clip, t);

	// ブレンド率
	float alpha = 1.0f;
	if (blendT > 0.0f) {
		alpha = std::clamp(t / blendT, 0.0f, 1.0f);
	}

	// ブレンドして適用
	MechAnimation::Pose outPose{};
	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		outPose.rotations[i] =
			Slerp(blendFromPose_.rotations[i], targetPose.rotations[i], alpha);
	}

	// Waistをブレンド
	outPose.waistTranslate =
		Lerp(blendFromPose_.waistTranslate, targetPose.waistTranslate, alpha);

	ApplyPose(outPose);
}

MechAnimation::Pose MechAnimator::CaptureCurrentPose() const {
	MechAnimation::Pose pose{};

	const Quaternion identity = MakeIdentityQuaternion();

	// waistTranslateの初期値
	pose.waistTranslate = Vector3{ 0.0f, 0.0f, 0.0f };

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		const auto type = static_cast<MechAnimation::TransType>(i);
		if (mech_) {
			Transform3D* trans = mech_->GetPartsTransform(type);
			if (trans) {
				pose.rotations[i] = trans->GetQuaternion(); // 現在回転
			} else {
				pose.rotations[i] = identity; // 無効はidentity
			}
		}
	}

	// Waistの位置を保存
	if (mech_) {
		Transform3D* waist = mech_->GetPartsTransform(MechAnimation::TransType::Waist);
		if (waist) {
			pose.waistTranslate = waist->GetTranslate();
		}
	}

	return pose;
}

void MechAnimator::ApplyPose(const MechAnimation::Pose& pose) {
	if (!mech_) {
		return;
	}

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		const auto type = static_cast<MechAnimation::TransType>(i);
		Transform3D* trans = mech_->GetPartsTransform(type);
		if (!trans) {
			continue;
		}

		// 回転反映
		trans->SetQuaternion(pose.rotations[i]);
	}

	// Waistの位置反映
	{
		Transform3D* waist = mech_->GetPartsTransform(MechAnimation::TransType::Waist);
		if (waist) {
			waist->SetTranslate(pose.waistTranslate); // 位置反映
		}
	}
}

MechAnimation::Pose MechAnimator::SampleClipPose(const MechAnimation::Clip& clip, float t) const {
	MechAnimation::Pose result{};

	const auto& frames = clip.frames;
	const int frameCount = static_cast<int>(frames.size());

	// 範囲クランプ
	t = std::clamp(t, 0.0f, 1.0f);

	// 1フレーム
	if (frameCount == 1) {
		return frames[0];
	}

	// フレーム位置
	const float pos = t * static_cast<float>(frameCount - 1);
	const int i0 = std::clamp(static_cast<int>(pos), 0, frameCount - 1);
	const int i1 = std::min(i0 + 1, frameCount - 1);
	const float localT = pos - static_cast<float>(i0);

	// Pose補間（回転）
	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		result.rotations[i] =
			Slerp(frames[static_cast<size_t>(i0)].rotations[i],
				  frames[static_cast<size_t>(i1)].rotations[i],
				  localT);
	}

	// Waist補間（平行移動）
	result.waistTranslate =
		Lerp(frames[static_cast<size_t>(i0)].waistTranslate,
			 frames[static_cast<size_t>(i1)].waistTranslate,
			 localT);

	return result;
}
