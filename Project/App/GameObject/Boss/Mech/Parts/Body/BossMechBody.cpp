#include "BossMechBody.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBody::BossMechBody(const BossMechBody::InitParam& param, BossMech* mech) {
	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// 初期化パラメータを受け取る
	bodyModelName_ = param.modelName;
	// モデル読み込み、Drawer作成
	MAGISYSTEM::LoadCreateModel(bodyModelName_);

	// トランスフォーム作成
	bodyTrans_ = MAGISYSTEM::AddTransform3D();

	// 親子付け
	bodyTrans_->SetParent(mech_->GetTransform());


}

void BossMechBody::Update() {

}

void BossMechBody::Draw() {
	// モデル描画
	MAGISYSTEM::DrawModel(bodyModelName_, bodyTrans_->GetWorldMatrix(), bodyMat_);
}

void BossMechBody::DebugDraw() {
	MAGISYSTEM::DrawLineSphere(bodyTrans_->GetWorldPosition(), 0.2f, Color::Red);
}

Transform3D* BossMechBody::GetTransform() {
	return bodyTrans_;
}
