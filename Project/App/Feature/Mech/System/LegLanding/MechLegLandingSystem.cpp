#include "MechLegLandingSystem.h"

#include <algorithm>

#include "Feature/Mech/Base/BaseMech.h"
#include "3D/Transform3D/Transform3D.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

#include "MAGI.h"

using namespace Magi;
using namespace MAGIMath;

namespace {
	// クランプ
	float Clamp(float v, float lo, float hi) {
		return std::clamp(v, lo, hi);
	}

	// ワールド回転取得
	Quaternion GetWorldRotation(Magi::Transform3D* t) {
		Vector3 s{}, tr{};
		Quaternion r{};
		MAGIMath::DecomposeAffineMatrix(t->GetWorldMatrix(), s, r, tr);
		return r;
	}

	// ワールド回転から、親に対するローカル回転を計算
	Quaternion MakeLocalFromWorldRotation(const Quaternion& targetWorldR, Magi::Transform3D* parent) {
		const Quaternion parentWorldR = GetWorldRotation(parent);
		const Quaternion parentInv = MAGIMath::Inverse(parentWorldR);
		return MAGIMath::Normalize(targetWorldR * parentInv);
	}
}

MechLegLandingSystem::MechLegLandingSystem(BaseMech* mech) {
	mech_ = mech;
	Initialize();
}

void MechLegLandingSystem::SetEnable(bool isEnable) {
	isEnable_ = isEnable;
}

void MechLegLandingSystem::SwitchEnable() {
	isEnable_ = !isEnable_;
}

void MechLegLandingSystem::SwitchDebugEnable() {
	isEnableDebug_ = !isEnableDebug_;
}

bool MechLegLandingSystem::IsWorking() const {
	return isWorking_;
}

void MechLegLandingSystem::Initialize() {
	if (!mech_) {
		return;
	}

	const LegRef legL = GetLegRefLeft();
	const LegRef legR = GetLegRefRight();

	// 初期値記録
	InitLeg(legL, footOffsetYL_, upperHeightYL_);
	InitLeg(legR, footOffsetYR_, upperHeightYR_);
}

void MechLegLandingSystem::Update() {

#if defined (DEBUG)|(DEVELOP)
	if (!isEnableDebug_) {
		return;
	}
#endif

	// このフレームの結果をリセット
	isWorking_ = false;

	if (!isEnable_) {
		return;
	}
	if (!mech_) {
		return;
	}

	// 接地していないなら何もしない
	if (!mech_->GetKinematicSystem()->IsGrounded()) {
		return;
	}

	const LegRef legL = GetLegRefLeft();
	const LegRef legR = GetLegRefRight();

	UpdateLeg(legL, footOffsetYL_, upperHeightYL_);
	UpdateLeg(legR, footOffsetYR_, upperHeightYR_);
}


MechLegLandingSystem::LegRef MechLegLandingSystem::GetLegRefLeft() const {
	LegRef leg{};
	leg.upper = mech_->GetPartsTransform(MechAnimation::TransType::UpperLegLeft);
	leg.lower = mech_->GetPartsTransform(MechAnimation::TransType::LowerLegLeft);
	leg.foot = mech_->GetPartsTransform(MechAnimation::TransType::FootLeft);
	return leg;
}

MechLegLandingSystem::LegRef MechLegLandingSystem::GetLegRefRight() const {
	LegRef leg{};
	leg.upper = mech_->GetPartsTransform(MechAnimation::TransType::UpperLegRight);
	leg.lower = mech_->GetPartsTransform(MechAnimation::TransType::LowerLegRight);
	leg.foot = mech_->GetPartsTransform(MechAnimation::TransType::FootRight);
	return leg;
}

void MechLegLandingSystem::InitLeg(const LegRef& leg, float& outFootOffsetY, float& outUpperHeightY) const {
	if (!mech_) {
		return;
	}
	if (!leg.upper || !leg.foot) {
		return;
	}

	// 機体基準（コライダー底の想定）
	const float baseY = mech_->GetTransform()->GetWorldPosition().y;

	// 足先の初期オフセット
	outFootOffsetY = leg.foot->GetWorldPosition().y - baseY;

	// UpperLeg の初期高さ
	outUpperHeightY = leg.upper->GetWorldPosition().y - baseY;
}

void MechLegLandingSystem::UpdateLeg(const LegRef& leg, float footOffsetY, float upperHeightY) {
	if (!mech_) {
		return;
	}
	if (!leg.upper || !leg.lower || !leg.foot) {
		return;
	}

	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	if (dt <= 0.0f) {
		return;
	}

	// 機体基準（コライダー底の想定）
	const float baseY = mech_->GetTransform()->GetWorldPosition().y;

	// UpperLeg 高さ（腰落ち判定）
	const float upperNowY = leg.upper->GetWorldPosition().y - baseY;

	// 初期より高い（浮いてる） or ほぼ同じなら触らない
	if (upperNowY >= upperHeightY - upperDropEps_) {
		return;
	}

	// 足先の目標高さ（初期オフセットを維持）
	const float footTargetY = baseY + footOffsetY;
	const float footNowY = leg.foot->GetWorldPosition().y;

	// 目標との差（+なら足が沈んでいる、-なら足が高い）
	const float errorY = footTargetY - footNowY;

	// 許容範囲なら何もしない
	if (std::fabs(errorY) <= footEps_) {
		return;
	}

	// このフレームで補正が動いた
	isWorking_ = true;

	// 60fps基準のパラメータを秒あたりへ変換
	const float gainPerSec = gain_ * 60.0f;
	const float maxStepPerSec = maxStepRad_ * 60.0f;

	// 誤差に応じて目標に近づける（fps非依存）
	const float step = Clamp(std::fabs(errorY) * gainPerSec * dt, 0.0f, maxStepPerSec * dt);

	// 誤差の符号で回転方向を決める
	float dir = 1.0f;
	if (errorY < 0.0f) {
		dir = -1.0f;
	}

	// 脚の曲げ量
	const float delta = step * dir;

	// 自然にかがむ向きへ
	leg.upper->AddRotate({ -delta * upperWeight_, 0.0f, 0.0f });
	leg.lower->AddRotate({ delta * lowerWeight_, 0.0f, 0.0f });

	// Foot のワールド回転を Waist と一致させる
	Magi::Transform3D* model = mech_->GetModelTransform();
	Magi::Transform3D* footParent = leg.foot->GetParent();
	if (model && footParent) {
		const Quaternion targetWorldR = GetWorldRotation(model);

		// 親に対するローカル回転へ変換してセット
		const Quaternion footLocalR = MakeLocalFromWorldRotation(targetWorldR, footParent);
		leg.foot->SetQuaternion(Inverse(footLocalR));
	}
}
