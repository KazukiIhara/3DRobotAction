#define NOMINMAX

#include "BossMechBarrier.h"

#include "Feature/Boss/Mech/BossMech.h"

#include "MAGI.h"

using namespace Magi;

BossMechBarrier::BossMechBarrier(BossMech* mech) {
	mech_ = mech;
	// パラメータをロード
	const float radius = MAGISYSTEM::GetParameterValue<float>({ "BossMechParam","Barrier","Radius" });

	// 設定
	transform_ = MAGISYSTEM::AddTransform3D();
	// 胴体に親子付け
	transform_->SetParent(mech->GetPartsTransform(MechAnimation::TransType::Body), false);

	// 形状を設定
	data_.radius = radius;

	// マテリアルの設定
	mat_.blendMode = BlendMode::Add;

	// コライダーを設定
	collider_.radius = radius;

}

void BossMechBarrier::Update() {
#if defined(DEBUG)|(DEVELOP)
	// パラメータをロード
	const float radius = MAGISYSTEM::GetParameterValue<float>({ "BossMechParam","Barrier","Radius" });
	// 形状を設定
	data_.radius = radius;
	// コライダーを設定
	collider_.radius = radius;
#endif

	// バリア破壊
	if (state_.hp <= 0.0f) {
		state_.isActive_ = false;
	}

	collider_.wPos = transform_->GetWorldPosition();
}

void BossMechBarrier::Draw() {
	MAGISYSTEM::DrawSphere3D(transform_->GetWorldMatrix(), data_, mat_);
}

void BossMechBarrier::Damage(float damage) {
	state_.hp -= damage;
	state_.hp = std::max(0.0f, state_.hp);
}

void BossMechBarrier::RepairBarrier() {
	// HPを回復
	state_.hp;
}

const BossMechBarrier::State& BossMechBarrier::GetState() {
	return state_;
}

const BossMechBarrier::Collider& BossMechBarrier::GetCollider() {
	return collider_;
}
