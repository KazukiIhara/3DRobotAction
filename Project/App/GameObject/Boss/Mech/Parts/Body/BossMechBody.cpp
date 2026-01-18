#include "BossMechBody.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBody::BossMechBody(const BossMechBody::InitParam& param, BossMech* mech) {
	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// トランスフォーム作成
	bodyTrans_ = MAGISYSTEM::AddTransform3D();

	// 腰と親子付け
	bodyTrans_->SetParent(mech_->GetTransform(), false);

	// 初期化パラメータ取得
	SetInitParam(param);
}

void BossMechBody::Update() {

}

void BossMechBody::Draw() {
	// モデル描画
	MAGISYSTEM::DrawModel(bodyModelName_, bodyTrans_->GetWorldMatrix(), bodyMat_);
}

void BossMechBody::DebugDraw() {
	const float debugSphereRadius = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });
	MAGISYSTEM::DrawLineSphere(bodyTrans_->GetWorldPosition(), debugSphereRadius, Color::Red);
}

void BossMechBody::SetInitParam(const InitParam& param) {
	// モデル名取得
	bodyModelName_ = param.modelName;

	// モデル読み込みDrawer作成
	MAGISYSTEM::LoadCreateModel(bodyModelName_);

	// トランスフォーム反映
	bodyTrans_->SetTranslate(param.translate);
}

void BossMechBody::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	bodyTrans_->SetTranslate(param.translate);
}

Transform3D* BossMechBody::GetTransform() {
	return bodyTrans_;
}
