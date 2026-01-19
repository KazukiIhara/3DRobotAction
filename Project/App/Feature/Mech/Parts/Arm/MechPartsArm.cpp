#include "MechPartsArm.h"

#include "MAGI.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/Base/BaseMech.h"

using namespace Magi;

MechPartsArm::MechPartsArm(const InitParam& param, BaseMech* mech) {
	// 機体参照を保持
	mech_ = mech;

	// トランスフォーム作成
	upperTrans_ = MAGISYSTEM::AddTransform3D();
	lowerTrans_ = MAGISYSTEM::AddTransform3D();
	handTrans_ = MAGISYSTEM::AddTransform3D();

	// 親子付け
	lowerTrans_->SetParent(upperTrans_, false);
	handTrans_->SetParent(lowerTrans_, false);

	// 胴体と紐づけ
	if (mech_ && mech_->GetBody() && mech_->GetBody()->GetTransform()) {
		upperTrans_->SetParent(mech_->GetBody()->GetTransform(), false);
	} else if (mech_ && mech_->GetTransform()) {
		upperTrans_->SetParent(mech_->GetTransform(), false);
	}

	// 初期化反映
	SetInitParam(param);
}

void MechPartsArm::Update() {
	// ここでは特に処理なし
}

void MechPartsArm::Draw() {
	// モデル描画
	MAGISYSTEM::DrawModel(upperModelName_, upperTrans_->GetWorldMatrix());
	MAGISYSTEM::DrawModel(lowerModelName_, lowerTrans_->GetWorldMatrix());
	MAGISYSTEM::DrawModel(handModelName_, handTrans_->GetWorldMatrix());
}

void MechPartsArm::DebugDraw() {
	// デバッグ球サイズ取得
	const float r = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });

	// デバッグ描画
	MAGISYSTEM::DrawLineSphere(upperTrans_->GetWorldPosition(), r, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTrans_->GetWorldPosition(), r, Color::Blue);
	MAGISYSTEM::DrawLineSphere(handTrans_->GetWorldPosition(), r, Color::Yellow);
}

void MechPartsArm::SetInitParam(const InitParam& param) {
	// モデル名反映
	upperModelName_ = param.upperModelName;
	lowerModelName_ = param.lowerModelName;
	handModelName_ = param.handModelName;

	// モデル読み込み
	MAGISYSTEM::LoadCreateModel(upperModelName_);
	MAGISYSTEM::LoadCreateModel(lowerModelName_);
	MAGISYSTEM::LoadCreateModel(handModelName_);

	// トランスフォーム反映
	upperTrans_->SetTranslate(param.upperTranslate);
	lowerTrans_->SetTranslate(param.lowerTranslate);
	handTrans_->SetTranslate(param.handTranslate);
}

void MechPartsArm::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	upperTrans_->SetTranslate(param.upperTranslate);
	lowerTrans_->SetTranslate(param.lowerTranslate);
	handTrans_->SetTranslate(param.handTranslate);
}

Transform3D* MechPartsArm::GetUpperTransform() { return upperTrans_; }
Transform3D* MechPartsArm::GetLowerTransform() { return lowerTrans_; }
Transform3D* MechPartsArm::GetHandTransform() { return handTrans_; }
