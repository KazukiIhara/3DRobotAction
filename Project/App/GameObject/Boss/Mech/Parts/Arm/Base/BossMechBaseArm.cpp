#include "BossMechBaseArm.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBaseArm::BossMechBaseArm(BossMech* mech) {
	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// トランスフォーム作成
	// 上腕
	upperTrans_ = MAGISYSTEM::AddTransform3D();
	// 下腕
	lowerTrans_ = MAGISYSTEM::AddTransform3D();
	lowerTrans_->SetParent(upperTrans_,false);
	// 手
	handTrans_ = MAGISYSTEM::AddTransform3D();
	handTrans_->SetParent(lowerTrans_, false);

	// 胴体に親子付け
	
}

void BossMechBaseArm::Update() {

}

void BossMechBaseArm::Draw() {
	MAGISYSTEM::DrawModel(upperModelName_, upperTrans_->GetWorldMatrix(), armMat_);
	MAGISYSTEM::DrawModel(lowerModelName_, lowerTrans_->GetWorldMatrix(), armMat_);
	MAGISYSTEM::DrawModel(handModelName_, handTrans_->GetWorldMatrix(), armMat_);
}

void BossMechBaseArm::DebugDraw() {
	MAGISYSTEM::DrawLineSphere(upperTrans_->GetWorldPosition(), 0.2f, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTrans_->GetWorldPosition(), 0.2f, Color::Blue);
	MAGISYSTEM::DrawLineSphere(handTrans_->GetWorldPosition(), 0.2f, Color::Yellow);
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