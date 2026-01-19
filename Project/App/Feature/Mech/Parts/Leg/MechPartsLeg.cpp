#include "MechPartsLeg.h"

#include "MAGI.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/Base/BaseMech.h"

using namespace Magi;

MechPartsLeg::MechPartsLeg(const InitParam& param, BaseMech* mech) {
	// 機体参照を保持
	mech_ = mech;

	// Transform作成
	waistTrans_ = MAGISYSTEM::AddTransform3D();

	upperTransLeft_ = MAGISYSTEM::AddTransform3D();
	lowerTransLeft_ = MAGISYSTEM::AddTransform3D();
	footTransLeft_ = MAGISYSTEM::AddTransform3D();

	upperTransRight_ = MAGISYSTEM::AddTransform3D();
	lowerTransRight_ = MAGISYSTEM::AddTransform3D();
	footTransRight_ = MAGISYSTEM::AddTransform3D();

	// 親子付け（Left）
	upperTransLeft_->SetParent(waistTrans_, false);
	lowerTransLeft_->SetParent(upperTransLeft_, false);
	footTransLeft_->SetParent(lowerTransLeft_, false);

	// 親子付け（Right）
	upperTransRight_->SetParent(waistTrans_, false);
	lowerTransRight_->SetParent(upperTransRight_, false);
	footTransRight_->SetParent(lowerTransRight_, false);

	// Mechと紐づけ
	if (mech_ && mech_->GetTransform()) {
		waistTrans_->SetParent(mech_->GetTransform(), false);
	}

	// 初期化反映
	SetInitParam(param);
}

void MechPartsLeg::Update() {
	// ここでは特に処理なし
}

void MechPartsLeg::Draw() {
	// 腰描画
	MAGISYSTEM::DrawModel(waistModelName_, waistTrans_->GetWorldMatrix());

	// Left描画
	MAGISYSTEM::DrawModel(upperModelNameLeft_, upperTransLeft_->GetWorldMatrix());
	MAGISYSTEM::DrawModel(lowerModelNameLeft_, lowerTransLeft_->GetWorldMatrix());
	MAGISYSTEM::DrawModel(footModelNameLeft_, footTransLeft_->GetWorldMatrix());

	// Right描画
	MAGISYSTEM::DrawModel(upperModelNameRight_, upperTransRight_->GetWorldMatrix());
	MAGISYSTEM::DrawModel(lowerModelNameRight_, lowerTransRight_->GetWorldMatrix());
	MAGISYSTEM::DrawModel(footModelNameRight_, footTransRight_->GetWorldMatrix());
}

void MechPartsLeg::DebugDraw() {
	// デバッグ球サイズ取得
	const float r = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });

	// Leftデバッグ
	MAGISYSTEM::DrawLineSphere(upperTransLeft_->GetWorldPosition(), r, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTransLeft_->GetWorldPosition(), r, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTransLeft_->GetWorldPosition(), r, Color::Yellow);

	// Rightデバッグ
	MAGISYSTEM::DrawLineSphere(upperTransRight_->GetWorldPosition(), r, Color::Red);
	MAGISYSTEM::DrawLineSphere(lowerTransRight_->GetWorldPosition(), r, Color::Blue);
	MAGISYSTEM::DrawLineSphere(footTransRight_->GetWorldPosition(), r, Color::Yellow);
}

void MechPartsLeg::SetInitParam(const InitParam& param) {
	// モデル名反映
	waistModelName_ = param.waistModelName;

	upperModelNameLeft_ = param.upperModelNameLeft;
	lowerModelNameLeft_ = param.lowerModelNameLeft;
	footModelNameLeft_ = param.footModelNameLeft;

	upperModelNameRight_ = param.upperModelNameRight;
	lowerModelNameRight_ = param.lowerModelNameRight;
	footModelNameRight_ = param.footModelNameRight;

	// モデル読み込み
	MAGISYSTEM::LoadCreateModel(waistModelName_);

	MAGISYSTEM::LoadCreateModel(upperModelNameLeft_);
	MAGISYSTEM::LoadCreateModel(lowerModelNameLeft_);
	MAGISYSTEM::LoadCreateModel(footModelNameLeft_);

	MAGISYSTEM::LoadCreateModel(upperModelNameRight_);
	MAGISYSTEM::LoadCreateModel(lowerModelNameRight_);
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

void MechPartsLeg::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	waistTrans_->SetTranslate(param.waistTranslate);

	upperTransLeft_->SetTranslate(param.upperTranslateLeft);
	lowerTransLeft_->SetTranslate(param.lowerTranslateLeft);
	footTransLeft_->SetTranslate(param.footTranslateLeft);

	upperTransRight_->SetTranslate(param.upperTranslateRight);
	lowerTransRight_->SetTranslate(param.lowerTranslateRight);
	footTransRight_->SetTranslate(param.footTranslateRight);
}

Transform3D* MechPartsLeg::GetWaistTransform() { return waistTrans_; }

Transform3D* MechPartsLeg::GetUpperTransformLeft() { return upperTransLeft_; }
Transform3D* MechPartsLeg::GetLowerTransformLeft() { return lowerTransLeft_; }
Transform3D* MechPartsLeg::GetFootTransformLeft() { return footTransLeft_; }

Transform3D* MechPartsLeg::GetUpperTransformRight() { return upperTransRight_; }
Transform3D* MechPartsLeg::GetLowerTransformRight() { return lowerTransRight_; }
Transform3D* MechPartsLeg::GetFootTransformRight() { return footTransRight_; }
