#include "MechPartsHead.h"

#include "MAGI.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/Mech/Base/BaseMech.h"

using namespace Magi;

MechPartsHead::MechPartsHead(const InitParam& param, BaseMech* mech) {
	// 機体参照を保持
	mech_ = mech;

	// トランスフォーム作成
	headTrans_ = MAGISYSTEM::AddTransform3D();

	// 体と紐づけ
	if (mech_ && mech_->GetBody() && mech_->GetBody()->GetTransform()) {
		headTrans_->SetParent(mech_->GetBody()->GetTransform(), false);
	} else if (mech_ && mech_->GetTransform()) {
		headTrans_->SetParent(mech_->GetTransform(), false);
	}

	// 初期化反映
	SetInitParam(param);
}

void MechPartsHead::Update() {
	// ここでは特に処理なし
}

void MechPartsHead::Draw() {
	// モデル描画
	MAGISYSTEM::DrawModel(headModelName_, headTrans_->GetWorldMatrix());
}

void MechPartsHead::DebugDraw() {
	// デバッグ球サイズ取得
	const float r = MAGISYSTEM::GetParameterValue<float>({ "MechInitParam","DebugSphere" });

	// デバッグ描画
	MAGISYSTEM::DrawLineSphere(headTrans_->GetWorldPosition(), r, Color::Red);
}

void MechPartsHead::SetInitParam(const InitParam& param) {
	// モデル名反映
	headModelName_ = param.modelName;

	// モデル読み込み
	MAGISYSTEM::LoadCreateModel(headModelName_);

	// トランスフォーム反映
	headTrans_->SetTranslate(param.translate);
}

void MechPartsHead::SetInitTranslate(const InitParam& param) {
	// トランスフォーム反映
	headTrans_->SetTranslate(param.translate);
}

Magi::Transform3D* MechPartsHead::GetHeadTransform() { return headTrans_; }
