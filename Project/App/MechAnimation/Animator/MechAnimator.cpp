#define NOMINMAX

#include "MechAnimator.h"

// C++
#include <algorithm>

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "Feature/Mech/Base/BaseMech.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/System/Kinematic/MechKinematicSystem.h"

#include "MAGI.h"

using namespace Magi;

using namespace MAGIMath;

MechAnimator::MechAnimator(MechAnimationContainer* container, BaseMech* mech) {
	container_ = container;
	mech_ = mech;

	// 全ジョイントをアニメ適用にする
	jointAnimEnabled_.fill(true);
}

bool MechAnimator::IsJointAnimationEnabled(MechAnimation::TransType joint) const {
	const size_t i = static_cast<size_t>(joint);
	if (i >= MechAnimation::kJointCount) {
		return true;
	}

	// ジョイント単位の無効を優先
	if (!jointAnimEnabled_[i]) {
		return false;
	}

	// Waist回転は専用フラグも見る
	if (joint == MechAnimation::TransType::Waist) {
		return isWaistRotAnimEnabled_;
	}

	// 頭回転は専用フラグも見る
	if (joint == MechAnimation::TransType::Head) {
		return isHeadRotAnimEnabled_;
	}

	// 体回転は専用フラグも見る
	if (joint == MechAnimation::TransType::Body) {
		return isBodyRotAnimEnabled_;
	}

	// 左腕回転は専用フラグも見る
	if (
		joint == MechAnimation::TransType::UpperArmLeft ||
		joint == MechAnimation::TransType::LowerArmLeft ||
		joint == MechAnimation::TransType::HandLeft
		) {
		return isLeftArmRotAnimEnabled_;
	}

	// 右腕回転は専用フラグも見る
	if (
		joint == MechAnimation::TransType::UpperArmRight ||
		joint == MechAnimation::TransType::LowerArmRight ||
		joint == MechAnimation::TransType::HandRight
		) {
		return isRightArmRotAnimEnabled_;
	}

	return true;
}

void MechAnimator::SetJointAnimationEnabled(MechAnimation::TransType joint, bool enabled) {
	const size_t i = static_cast<size_t>(joint);
	if (i >= MechAnimation::kJointCount) {
		return;
	}

	// 指定ジョイントのアニメ適用を切り替え
	jointAnimEnabled_[i] = enabled;
}

void MechAnimator::SetLegAnimationEnabled(bool enabled) {
	// 左脚
	SetJointAnimationEnabled(MechAnimation::TransType::UpperLegLeft, enabled);
	SetJointAnimationEnabled(MechAnimation::TransType::LowerLegLeft, enabled);
	SetJointAnimationEnabled(MechAnimation::TransType::FootLeft, enabled);

	// 右脚
	SetJointAnimationEnabled(MechAnimation::TransType::UpperLegRight, enabled);
	SetJointAnimationEnabled(MechAnimation::TransType::LowerLegRight, enabled);
	SetJointAnimationEnabled(MechAnimation::TransType::FootRight, enabled);
}

void MechAnimator::SetAutoDisableLegOnGround(bool enabled) {
	// 接地時に脚アニメを止めるか
	autoDisableLegOnGround_ = enabled;
}

void MechAnimator::SetWaistRotationAnimationEnabled(bool enabled) {
	// Waist回転アニメの有効/無効を切り替える
	isWaistRotAnimEnabled_ = enabled;
}

bool MechAnimator::IsWaistRotationAnimationEnabled() const {
	// Waist回転アニメが有効か返す
	return isWaistRotAnimEnabled_;
}

void MechAnimator::SetHeadRotationAnimationEnabled(bool enabled) {
	// 頭回転アニメの有効/無効を切り替える
	isHeadRotAnimEnabled_ = enabled;
}

bool MechAnimator::IsHeadRotationAnimationEnabled() const {
	// 頭回転アニメが有効か返す
	return isHeadRotAnimEnabled_;
}

void MechAnimator::SetBodyRotationAnimationEnabled(bool enabled) {
	// 体回転アニメの有効/無効を切り替える
	isBodyRotAnimEnabled_ = enabled;
}

bool MechAnimator::IsBodyRotationAnimationEnabled() const {
	// 体回転アニメが有効か返す
	return isBodyRotAnimEnabled_;
}

void MechAnimator::SetLeftArmRotationAnimationEnabled(bool enabled) {
	// 左腕回転アニメの有効/無効を切り替える
	isLeftArmRotAnimEnabled_ = enabled;
}

bool MechAnimator::IsLeftArmRotationAnimationEnabled() const {
	// 左腕回転アニメが有効か返す
	return isLeftArmRotAnimEnabled_;
}

void MechAnimator::SetRightArmRotationAnimationEnabled(bool enabled) {
	// 右腕回転アニメの有効/無効を切り替える
	isRightArmRotAnimEnabled_ = enabled;
}

bool MechAnimator::IsRightArmRotationAnimationEnabled() const {
	// 右腕回転アニメが有効か返す
	return isRightArmRotAnimEnabled_;
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

void MechAnimator::PlayAnimation(
	const std::string& name,
	float durationSec,
	float blendSec,
	EasingType easing,
	MechAnimation::LoopType loopType
) {
	if (!container_ || !mech_) {
		return;
	}

	const MechAnimation::Clip* clip = container_->GetClip(name);
	if (!clip || clip->frames.empty()) {
		return;
	}

	playingClipName_ = name;
	durationSec_ = std::max(0.0f, durationSec);
	blendSec_ = std::max(0.0f, blendSec);
	easing_ = easing;
	loopType_ = loopType;

	// 現在姿勢をブレンド開始姿勢として保持
	blendFromPose_ = CaptureCurrentPose();

	playTimeSec_ = 0.0f;
	isPlaying_ = true;
	pingPongForward_ = true;

	if (durationSec_ <= 0.0f) {
		Update();
		isPlaying_ = false;
	}
}

void MechAnimator::StopAnimation() {
	isPlaying_ = false;
}

void MechAnimator::ApproachPose(const std::string& clipName, float reachSecond) {
	if (!container_ || !mech_) {
		return;
	}

	// クリップ取得
	const MechAnimation::Clip* clip = container_->GetClip(clipName);
	if (!clip) {
		return;
	}

	// 1フレーム専用
	if (clip->frames.size() != 1) {
		return;
	}

	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 現在Pose取得
	const MechAnimation::Pose currentPose = CaptureCurrentPose();

	// ターゲットPose取得
	const MechAnimation::Pose& targetPose = clip->frames[0];

	// 指数補間係数
	float expT = CalExpT(dt, reachSecond, 1.0f);

	MechAnimation::Pose outPose{};

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {

		const auto type = static_cast<MechAnimation::TransType>(i);

		// 無効ジョイントは動かさない
		if (!IsJointAnimationEnabled(type)) {
			outPose.rotations[i] = currentPose.rotations[i];
			continue;
		}

		// 回転をターゲットへ近づける
		outPose.rotations[i] = Slerp(currentPose.rotations[i], targetPose.rotations[i], expT);
	}

	// Waist位置をターゲットへ近づける
	outPose.waistTranslate = Lerp(currentPose.waistTranslate, targetPose.waistTranslate, expT);

	// 適用
	ApplyPose(outPose);
}

void MechAnimator::Update() {

	const float dt = MAGISYSTEM::GetDeltaTime();

	// 接地中は脚アニメを止める
	if (autoDisableLegOnGround_) {
		MechKinematicSystem* kin = mech_->GetKinematicSystem();
		if (kin) {
			SetLegAnimationEnabled(!kin->IsGrounded());
		}
	}

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

	// 正規化時間（まずは生の比率を作る）
	float t = 1.0f;

	// duration がある場合のみ計算
	if (durationSec_ > 0.0f) {
		t = playTimeSec_ / durationSec_;
	}

	// ループ処理
	switch (loopType_) {
	case MechAnimation::LoopType::None: {
		t = std::clamp(t, 0.0f, 1.0f);
		break;
	}

	case MechAnimation::LoopType::Restart: {
		// duration=0 の場合は常に終端
		if (durationSec_ <= 0.0f) {
			t = 1.0f;
			break;
		}

		// 1周したら先頭へ
		if (t >= 1.0f) {
			playTimeSec_ = std::fmod(playTimeSec_, durationSec_);
			t = playTimeSec_ / durationSec_;
		}

		t = std::clamp(t, 0.0f, 1.0f);
		break;
	}

	case MechAnimation::LoopType::PingPong: {
		// duration=0 の場合は常に終端
		if (durationSec_ <= 0.0f) {
			t = 1.0f;
			break;
		}

		// 端で反転
		if (t >= 1.0f) {
			playTimeSec_ = durationSec_;
			pingPongForward_ = false;
		} else if (t <= 0.0f && !pingPongForward_) {
			playTimeSec_ = 0.0f;
			pingPongForward_ = true;
		}

		t = std::clamp(t, 0.0f, 1.0f);

		// 逆向きなら 1->0 に変換
		if (!pingPongForward_) {
			t = 1.0f - t;
		}
		break;
	}
	}

	// イージング適用
	const float easedT = std::clamp(Easing::Apply(easing_, t), 0.0f, 1.0f);

	// ターゲット姿勢
	const MechAnimation::Pose targetPose = SampleClipPose(*clip, easedT);

	// ブレンド率（開始姿勢→ターゲット）
	float alpha = 1.0f;

	// ブレンド秒がある場合のみ計算
	if (blendSec_ > 0.0f) {
		alpha = playTimeSec_ / blendSec_;
		alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	MechAnimation::Pose outPose{};
	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		// 無効ジョイントは現姿勢を維持するため blendFromPose を使う
		if (!IsJointAnimationEnabled(static_cast<MechAnimation::TransType>(i))) {
			outPose.rotations[i] = blendFromPose_.rotations[i];
			continue;
		}

		outPose.rotations[i] = Slerp(blendFromPose_.rotations[i], targetPose.rotations[i], alpha);
	}
	outPose.waistTranslate = Lerp(blendFromPose_.waistTranslate, targetPose.waistTranslate, alpha);

	// 適用
	ApplyPose(outPose);

	// 終了
	if (loopType_ == MechAnimation::LoopType::None && t >= 1.0f) {
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

		// 無効ジョイントはアニメを書き込まない
		if (!IsJointAnimationEnabled(type)) {
			continue;
		}

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