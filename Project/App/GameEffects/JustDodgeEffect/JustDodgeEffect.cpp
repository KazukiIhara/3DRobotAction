#include "JustDodgeEffect.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"

using namespace Magi;

JustDodgeEffect::JustDodgeEffect(const Vector3& emitPos, MechCore* mech) :
	BaseGameEffect(emitPos) {
	mech_ = mech;
	// エフェクトのタイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });

	// 各トランスフォーム作成、マネージャに突っ込む
	std::unique_ptr<Transform3D> ringTrans = std::make_unique<Transform3D>(worldPos_);
	ringTrans_ = MAGISYSTEM::AddTransform3D(std::move(ringTrans));

	std::unique_ptr<Transform3D> planeTrans = std::make_unique<Transform3D>(worldPos_);
	planeTrans_ = MAGISYSTEM::AddTransform3D(std::move(planeTrans));

	// パーティクル発生

}

JustDodgeEffect::~JustDodgeEffect() {
	// トランスフォームの削除など



}

void JustDodgeEffect::Update() {
	const float kEffectTime = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });

}

void JustDodgeEffect::Draw() {
	// リング描画
	MAGISYSTEM::DrawRing3D(ringTrans_->GetWorldMatrix(), ringData_, ringMat_);
	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(planeTrans_->GetWorldMatrix(), planeData_, planeMat_);
}
