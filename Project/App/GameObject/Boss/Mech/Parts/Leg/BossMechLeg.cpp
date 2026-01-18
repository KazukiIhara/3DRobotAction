#include "BossMechLeg.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechLeg::BossMechLeg(const BossMechLeg::InitParam& param, BossMech* mech) {
	// 機体のポインタを受け取る
	mech_ = mech;

	// Transform作成
	waistTrans_ = MAGISYSTEM::AddTransform3D();

	upperTransLeft_ = MAGISYSTEM::AddTransform3D();
	upperTransLeft_->SetParent(waistTrans_, false);
	lowerTransLeft_ = MAGISYSTEM::AddTransform3D();
	lowerTransLeft_->SetParent(upperTransLeft_, false);
	footTransLeft_ = MAGISYSTEM::AddTransform3D();
	footTransLeft_->SetParent(lowerTransLeft_, false);

	upperTransRight_ = MAGISYSTEM::AddTransform3D();
	upperTransRight_->SetParent(waistTrans_, false);
	lowerTransRight_ = MAGISYSTEM::AddTransform3D();
	lowerTransRight_->SetParent(upperTransRight_, false);
	footTransRight_ = MAGISYSTEM::AddTransform3D();
	footTransRight_->SetParent(lowerTransRight_, false);

	// 胴体と紐づけ
	waistTrans_->SetParent(mech->GetBody()->GetTransform(), false);

	SetInitParam(param);

}

void BossMechLeg::Update() {

}

void BossMechLeg::Draw() {
	// 腰
	MAGISYSTEM::DrawModel(waistModelName_, waistTrans_->GetWorldMatrix(), legMat_);

	// Left
	MAGISYSTEM::DrawModel(upperModelNameLeft_, upperTransLeft_->GetWorldMatrix(), legMat_);
	MAGISYSTEM::DrawModel(lowerModelNameLeft_, lowerTransLeft_->GetWorldMatrix(), legMat_);
	MAGISYSTEM::DrawModel(footModelNameLeft_, footTransLeft_->GetWorldMatrix(), legMat_);

	// Right
	MAGISYSTEM::DrawModel(upperModelNameRight_, upperTransRight_->GetWorldMatrix(), legMat_);
	MAGISYSTEM::DrawModel(lowerModelNameRight_, lowerTransRight_->GetWorldMatrix(), legMat_);
	MAGISYSTEM::DrawModel(footModelNameRight_, footTransRight_->GetWorldMatrix(), legMat_);
}

void BossMechLeg::DebugDraw() {
	// デバッグ用スフィアのサイズ
	const float debugSphereRadius = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });

	// Left
	MAGISYSTEM::DrawLineSphere(upperTransLeft_->GetWorldPosition(), debugSphereRadius, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTransLeft_->GetWorldPosition(), debugSphereRadius, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTransLeft_->GetWorldPosition(), debugSphereRadius, Color::Yellow);

	// Right
	MAGISYSTEM::DrawLineSphere(upperTransRight_->GetWorldPosition(), debugSphereRadius, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTransRight_->GetWorldPosition(), debugSphereRadius, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTransRight_->GetWorldPosition(), debugSphereRadius, Color::Yellow);

}

void BossMechLeg::SetInitParam(const InitParam& param) {
	// モデル名取得
	waistModelName_ = param.waistModelName;

	upperModelNameLeft_ = param.upperModelNameLeft;
	upperModelNameRight_ = param.upperModelNameRight;

	lowerModelNameLeft_ = param.lowerModelNameLeft;
	lowerModelNameRight_ = param.lowerModelNameRight;

	footModelNameLeft_ = param.footModelNameLeft;
	footModelNameRight_ = param.footModelNameRight;

	// モデル読み込みDrawer作成
	MAGISYSTEM::LoadCreateModel(waistModelName_);

	MAGISYSTEM::LoadCreateModel(upperModelNameLeft_);
	MAGISYSTEM::LoadCreateModel(upperModelNameRight_);

	MAGISYSTEM::LoadCreateModel(lowerModelNameLeft_);
	MAGISYSTEM::LoadCreateModel(lowerModelNameRight_);

	MAGISYSTEM::LoadCreateModel(footModelNameLeft_);
	MAGISYSTEM::LoadCreateModel(footModelNameRight_);

	// トランスフォーム反映
	waistTrans_->SetTranslate(param.waistTranslate);

	upperTransLeft_->SetTranslate(param.upperTranslateLeft);
	lowerTransLeft_->SetTranslate(param.lowerTranslateLeft);
	footTransLeft_->SetTranslate(param.footTranslateLeft);

	upperTransRight_->SetTranslate(param.upperTranslateRight);
	lowerTransRight_->SetTranslate(param.lowerTranslateRight);
	footTransRight_->SetTranslate(param.footTranslateRight);
}

void BossMechLeg::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	waistTrans_->SetTranslate(param.waistTranslate);

	upperTransLeft_->SetTranslate(param.upperTranslateLeft);
	lowerTransLeft_->SetTranslate(param.lowerTranslateLeft);
	footTransLeft_->SetTranslate(param.footTranslateLeft);

	upperTransRight_->SetTranslate(param.upperTranslateRight);
	lowerTransRight_->SetTranslate(param.lowerTranslateRight);
	footTransRight_->SetTranslate(param.footTranslateRight);
}


Transform3D* BossMechLeg::GetWaistTransform() {
	return waistTrans_;
}

Transform3D* BossMechLeg::GetUpperTransformLeft() {
	return upperTransLeft_;
}

Transform3D* BossMechLeg::GetUpperTransformRight() {
	return upperTransRight_;
}

Transform3D* BossMechLeg::GetLowerTransformLeft() {
	return lowerTransLeft_;
}

Transform3D* BossMechLeg::GetLowerTransformRight() {
	return lowerTransRight_;
}

Transform3D* BossMechLeg::GetFootTransformLeft() {
	return footTransLeft_;
}

Transform3D* BossMechLeg::GetFootTransformRight() {
	return footTransRight_;
}
