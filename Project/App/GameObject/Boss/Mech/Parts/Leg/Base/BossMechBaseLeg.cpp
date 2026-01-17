#include "BossMechBaseLeg.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBaseLeg::BossMechBaseLeg(const BossMechBaseLeg::InitParam& param, BossMech* mech) {
	// 初期化パラメータを受け取る
	upperModelName_ = param.upperModelName;
	lowerModelName_ = param.lowerModelName;
	footModelName_ = param.footModelName;

	// 機体のポインタを受け取る
	mech_ = mech;

	// トランスフォーム作成
	// 上足
	upperTrans_ = MAGISYSTEM::AddTransform3D();
	// 下足
	lowerTrans_ = MAGISYSTEM::AddTransform3D();
	lowerTrans_->SetParent(upperTrans_, false);
	// 足
	footTrans_ = MAGISYSTEM::AddTransform3D();
	footTrans_->SetParent(lowerTrans_, false);

}

void BossMechBaseLeg::Update() {

}

void BossMechBaseLeg::Draw() {

}

void BossMechBaseLeg::DebugDraw() {
	MAGISYSTEM::DrawLineSphere(upperTrans_->GetWorldPosition(), 0.2f, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTrans_->GetWorldPosition(), 0.2f, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTrans_->GetWorldPosition(), 0.2f, Color::Yellow);
}

Transform3D* BossMechBaseLeg::GetUpperTransform() {
	return upperTrans_;
}

Transform3D* BossMechBaseLeg::GetLowerTransform() {
	return lowerTrans_;
}

Transform3D* BossMechBaseLeg::GetFootTransform() {
	return footTrans_;
}