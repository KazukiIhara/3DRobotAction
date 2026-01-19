#include "MechPartsBody.h"

#include "MAGI.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/Base/BaseMech.h"

using namespace Magi;

MechPartsBody::MechPartsBody(const InitParam& param, BaseMech* mech) {
	// 機体参照を保持
	mech_ = mech;

	// トランスフォーム作成
	bodyTrans_ = MAGISYSTEM::AddTransform3D();

	// 腰と親子付け
	if (mech_ && mech_->GetLeg() && mech_->GetLeg()->GetWaistTransform()) {
		bodyTrans_->SetParent(mech_->GetLeg()->GetWaistTransform(), false);
	} else if (mech_ && mech_->GetTransform()) {
		bodyTrans_->SetParent(mech_->GetTransform(), false);
	}

	// 初期化反映
	SetInitParam(param);
}

void MechPartsBody::Update() {
	// ここでは特に処理なし
}

void MechPartsBody::Draw() {
	// モデル描画
	MAGISYSTEM::DrawModel(bodyModelName_, bodyTrans_->GetWorldMatrix());
}

void MechPartsBody::DebugDraw() {
	// デバッグ球サイズ取得
	const float r = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });

	// デバッグ描画
	MAGISYSTEM::DrawLineSphere(bodyTrans_->GetWorldPosition(), r, Color::Red);
}

void MechPartsBody::SetInitParam(const InitParam& param) {
	// モデル名反映
	bodyModelName_ = param.modelName;

	// モデル読み込み
	MAGISYSTEM::LoadCreateModel(bodyModelName_);

	// トランスフォーム反映
	bodyTrans_->SetTranslate(param.translate);
}

void MechPartsBody::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	bodyTrans_->SetTranslate(param.translate);
}

Transform3D* MechPartsBody::GetTransform() { return bodyTrans_; }
