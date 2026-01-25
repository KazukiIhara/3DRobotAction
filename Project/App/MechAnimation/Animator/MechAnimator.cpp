#define NOMINMAX

#include "MechAnimator.h"

// C++
#include <algorithm>

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "Feature/Mech/Base/BaseMech.h"
#include "3D/Transform3D/Transform3D.h"

using namespace MAGIMath;

MechAnimator::MechAnimator(MechAnimationContainer* container, BaseMech* mech) {
	container_ = container;
	mech_ = mech;
}

void MechAnimator::ApplyAnimation(const std::string& name, float t) {
	if (!container_ || !mech_) {
		return;
	}

	const MechAnimation::Clip* clip = container_->GetClip(name);
	if (!clip || clip->frames.empty()) {
		return;
	}

	// 範囲クランプ
	t = std::clamp(t, 0.0f, 1.0f);

	// クリップをサンプルして適用
	const MechAnimation::Pose pose = SampleClipPose(*clip, t);
	ApplyPose(pose);
}

void MechAnimator::PlayAnimation(const std::string& name, float durationSec, float blendSec, EasingType easing) {
	if (!container_ || !mech_) {
		return;
	}

	const MechAnimation::Clip* clip = container_->GetClip(name);
	if (!clip || clip->frames.empty()) {
		return;
	}

	// 再生情報を保存
	playingClipName_ = name;
	durationSec_ = std::max(0.0f, durationSec);
	blendSec_ = std::max(0.0f, blendSec);
	easing_ = easing;

	// 再生開始姿勢を保存
	blendFromPose_ = CaptureCurrentPose();

	// 再生開始
	playTimeSec_ = 0.0f;
	isPlaying_ = true;

	// duration=0なら即終端適用
	if (durationSec_ <= 0.0f) {
		Update(0.0f);
		isPlaying_ = false;
	}
}

void MechAnimator::StopAnimation() {
	isPlaying_ = false;
}

void MechAnimator::Update(float dt) {
	if (!isPlaying_) {
		return;
	}
	if (!container_ || !mech_) {
		return;
	}
	if (playingClipName_.empty()) {
		return;
	}

	const MechAnimation::Clip* clip = container_->GetClip(playingClipName_);
	if (!clip || clip->frames.empty()) {
		isPlaying_ = false;
		return;
	}

	// 経過時間
	playTimeSec_ += std::max(0.0f, dt);

	// 正規化時間
	const float t = (durationSec_ <= 0.0f)
		? 1.0f
		: std::clamp(playTimeSec_ / durationSec_, 0.0f, 1.0f);

	// イージング適用
	const float easedT = std::clamp(Easing::Apply(easing_, t), 0.0f, 1.0f);

	// ターゲット姿勢
	const MechAnimation::Pose targetPose = SampleClipPose(*clip, easedT);

	// ブレンド率（開始姿勢→ターゲット）
	const float alpha = (blendSec_ <= 0.0f)
		? 1.0f
		: std::clamp(playTimeSec_ / blendSec_, 0.0f, 1.0f);

	MechAnimation::Pose outPose{};
	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		outPose.rotations[i] = Slerp(blendFromPose_.rotations[i], targetPose.rotations[i], alpha);
	}
	outPose.waistTranslate = Lerp(blendFromPose_.waistTranslate, targetPose.waistTranslate, alpha);

	// 適用
	ApplyPose(outPose);

	// 終了
	if (t >= 1.0f) {
		isPlaying_ = false;
	}
}

MechAnimation::Pose MechAnimator::CaptureCurrentPose() const {
	MechAnimation::Pose pose{};

	// waistTranslate初期化
	pose.waistTranslate = Vector3{ 0.0f, 0.0f, 0.0f };

	const Quaternion identity = MakeIdentityQuaternion();

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		const auto type = static_cast<MechAnimation::TransType>(i);

		Transform3D* trans = mech_->GetPartsTransform(type);
		pose.rotations[i] = trans ? trans->GetQuaternion() : identity;
	}

	// Waistの位置
	{
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

	// Waist位置反映
	{
		Transform3D* waist = mech_->GetPartsTransform(MechAnimation::TransType::Waist);
		if (waist) {
			waist->SetTranslate(pose.waistTranslate);
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

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		result.rotations[i] = Slerp(
			frames[static_cast<size_t>(i0)].rotations[i],
			frames[static_cast<size_t>(i1)].rotations[i],
			localT
		);
	}

	result.waistTranslate = Lerp(
		frames[static_cast<size_t>(i0)].waistTranslate,
		frames[static_cast<size_t>(i1)].waistTranslate,
		localT
	);

	return result;
}
