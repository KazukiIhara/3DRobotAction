#include "BossMechLeg.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechLeg::BossMechLeg(const BossMechLeg::InitParam& param, BossMech* mech) {
	// 初期化パラメータを受け取る
	waistModelName_ = param.waistModelName;

	upperModelNameLeft_ = param.upperModelNameLeft;
	upperModelNameRight_ = param.upperModelNameRight;

	lowerModelNameLeft_ = param.lowerModelNameLeft;
	lowerModelNameRight_ = param.lowerModelNameRight;

	footModelNameLeft_ = param.footModelNameLeft;
	footModelNameRight_ = param.footModelNameRight;

	// 機体のポインタを受け取る
	mech_ = mech;

	// Transform作成
	waistTrans_ = MAGISYSTEM::AddTransform3D();


	upperTransLeft_ = MAGISYSTEM::AddTransform3D();
	upperTransLeft_->SetParent(waistTrans_);
	lowerTransLeft_ = MAGISYSTEM::AddTransform3D();
	lowerTransLeft_->SetParent(upperTransLeft_, false);
	footTransLeft_ = MAGISYSTEM::AddTransform3D();
	footTransLeft_->SetParent(lowerTransLeft_, false);

	upperTransRight_ = MAGISYSTEM::AddTransform3D();
	upperTransRight_->SetParent(waistTrans_);
	lowerTransRight_ = MAGISYSTEM::AddTransform3D();
	lowerTransRight_->SetParent(upperTransRight_, false);
	footTransRight_ = MAGISYSTEM::AddTransform3D();
	footTransRight_->SetParent(lowerTransRight_, false);
}

void BossMechLeg::Update() {

}

void BossMechLeg::Draw() {

}

void BossMechLeg::DebugDraw() {
	// Left
	MAGISYSTEM::DrawLineSphere(upperTransLeft_->GetWorldPosition(), 0.2f, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTransLeft_->GetWorldPosition(), 0.2f, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTransLeft_->GetWorldPosition(), 0.2f, Color::Yellow);

	// Right
	MAGISYSTEM::DrawLineSphere(upperTransRight_->GetWorldPosition(), 0.2f, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTransRight_->GetWorldPosition(), 0.2f, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTransRight_->GetWorldPosition(), 0.2f, Color::Yellow);
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
