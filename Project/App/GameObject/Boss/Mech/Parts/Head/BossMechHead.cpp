#include "BossMechHead.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechHead::BossMechHead(const BossMechHead::InitParam& param, BossMech* mech) {
	// 機体のポインタを受け取る
	mech_ = mech;

	// トランスフォーム作成
	headTrans_ = MAGISYSTEM::AddTransform3D();

	// 体と紐づけ
	headTrans_->SetParent(mech->GetBody()->GetTransform(), false);

	SetInitParam(param);
}

void BossMechHead::Update() {

}

void BossMechHead::Draw() {
	MAGISYSTEM::DrawModel(headModelName_, headTrans_->GetWorldMatrix(), headMat_);
}

void BossMechHead::DebugDraw() {
	const float debugSphereRadius = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });
	MAGISYSTEM::DrawLineSphere(headTrans_->GetWorldPosition(), debugSphereRadius, Color::Red);
}

void BossMechHead::SetInitParam(const InitParam& param) {
	// モデル名取得
	headModelName_ = param.modelName;

	// モデル読み込みDrawer作成
	MAGISYSTEM::LoadCreateModel(headModelName_);

	// トランスフォーム反映
	headTrans_->SetTranslate(param.translate);
}

void BossMechHead::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	headTrans_->SetTranslate(param.translate);
}

Transform3D* BossMechHead::GetHeadTransform() {
	return headTrans_;
}

