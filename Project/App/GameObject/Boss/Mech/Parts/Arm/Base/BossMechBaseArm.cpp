#include "BossMechBaseArm.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBaseArm::BossMechBaseArm(const BossMechBaseArm::InitParam& param, BossMech* mech) {
	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// トランスフォーム作成
	// 上腕
	upperTrans_ = MAGISYSTEM::AddTransform3D();
	// 下腕
	lowerTrans_ = MAGISYSTEM::AddTransform3D();
	lowerTrans_->SetParent(upperTrans_, false);
	// 手
	handTrans_ = MAGISYSTEM::AddTransform3D();
	handTrans_->SetParent(lowerTrans_, false);

	// 胴体と紐づけ
	upperTrans_->SetParent(mech->GetBody()->GetTransform(), false);

	// 初期化パラメータを受け取る
	SetInitParam(param);

}

void BossMechBaseArm::Update() {

}

void BossMechBaseArm::Draw() {
	MAGISYSTEM::DrawModel(upperModelName_, upperTrans_->GetWorldMatrix(), armMat_);
	MAGISYSTEM::DrawModel(lowerModelName_, lowerTrans_->GetWorldMatrix(), armMat_);
	MAGISYSTEM::DrawModel(handModelName_, handTrans_->GetWorldMatrix(), armMat_);
}

void BossMechBaseArm::DebugDraw() {
	const float debugSphereRadius = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });
	MAGISYSTEM::DrawLineSphere(upperTrans_->GetWorldPosition(), debugSphereRadius, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTrans_->GetWorldPosition(),debugSphereRadius, Color::Blue);
	MAGISYSTEM::DrawLineSphere(handTrans_->GetWorldPosition(),debugSphereRadius, Color::Yellow);
}

void BossMechBaseArm::SetInitParam(const InitParam& param) {
	// モデル名取得
	upperModelName_ = param.upperModelName;
	lowerModelName_ = param.lowerModelName;
	handModelName_ = param.handModelName;

	// モデル読み込みDrawer作成
	MAGISYSTEM::LoadCreateModel(upperModelName_);
	MAGISYSTEM::LoadCreateModel(lowerModelName_);
	MAGISYSTEM::LoadCreateModel(handModelName_);

	upperTrans_->SetTranslate(param.upperTranslate);
	lowerTrans_->SetTranslate(param.lowerTranslate);
	handTrans_->SetTranslate(param.handTranslate);
}

void BossMechBaseArm::SetInitTranslate(const InitParam& param) {
	upperTrans_->SetTranslate(param.upperTranslate);
	lowerTrans_->SetTranslate(param.lowerTranslate);
	handTrans_->SetTranslate(param.handTranslate);
}

Transform3D* BossMechBaseArm::GetUpperTransform() {
	return upperTrans_;
}

Transform3D* BossMechBaseArm::GetLowerTransform() {
	return lowerTrans_;
}

Transform3D* BossMechBaseArm::GetHandTransform() {
	return handTrans_;
}