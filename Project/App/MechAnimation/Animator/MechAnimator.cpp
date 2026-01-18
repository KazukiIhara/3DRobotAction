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

void MechAnimator::SetContainer(MechAnimationContainer* container) {
	container_ = container;
}

void MechAnimator::SetBossMech(BossMech* mech) {
	mech_ = mech;
}

bool MechAnimator::SetClip(const std::string& name) {
	if (!container_) {
		return false;
	}
	if (!container_->HasClip(name)) {
		return false;
	}
	clipName_ = name;
	return true;
}

const std::string& MechAnimator::GetClipName() const {
	return clipName_;
}

const MechAnimation::Clip* MechAnimator::GetCurrentClip() const {
	if (!container_) {
		return nullptr;
	}
	if (clipName_.empty()) {
		return nullptr;
	}
	return container_->GetClip(clipName_);
}

void MechAnimator::ApplyAnimation(float t) {
	const MechAnimation::Clip* clip = GetCurrentClip();
	if (!clip) {
		return;
	}

	const auto& frames = clip->frames;
	const int frameCount = static_cast<int>(frames.size());
	if (frameCount <= 0) {
		return;
	}

	// 範囲クランプ
	t = std::clamp(t, 0.0f, 1.0f);

	// 1フレームならそのまま適用
	if (frameCount == 1) {
		ApplyPose(frames[0]);
		return;
	}

	// t をフレーム位置へ変換
	const float pos = t * static_cast<float>(frameCount - 1);
	const int i0 = std::clamp(static_cast<int>(pos), 0, frameCount - 1);
	const int i1 = std::min(i0 + 1, frameCount - 1);
	const float localT = pos - static_cast<float>(i0);

	// 補間して適用
	ApplyPoseLerp(frames[static_cast<size_t>(i0)], frames[static_cast<size_t>(i1)], localT);
}

void MechAnimator::ApplyPose(const MechAnimation::Pose& pose) {
	if (!mech_) {
		return;
	}

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		// 対応する関節を取得
		const auto type = static_cast<BossMech::TransType>(i);
		Transform3D* trans = mech_->GetPartsTransform(type);
		if (!trans) {
			continue;
		}

		// 回転を適用
		trans->SetQuaternion(pose.rotations[i]);
	}
}

void MechAnimator::ApplyPoseLerp(const MechAnimation::Pose& a, const MechAnimation::Pose& b, float t) {
	if (!mech_) {
		return;
	}

	// 補間率クランプ
	t = std::clamp(t, 0.0f, 1.0f);

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		// 対応する関節を取得
		const auto type = static_cast<BossMech::TransType>(i);
		Transform3D* trans = mech_->GetPartsTransform(type);
		if (!trans) {
			continue;
		}

		// クォータニオン補間
		const Quaternion q = MAGIMath::Slerp(a.rotations[i], b.rotations[i], t);

		// 回転を適用
		trans->SetQuaternion(q);
	}
}
