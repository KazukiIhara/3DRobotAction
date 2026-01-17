#include "BossMechBody.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBody::BossMechBody(const BossMechBody::InitParam& param, BossMech* mech) {
	// 初期化パラメータを受け取る
	bodyModelName_ = param.modelName;

	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// トランスフォーム作成
	bodyTrans_ = MAGISYSTEM::AddTransform3D();
}

void BossMechBody::Update() {

}

void BossMechBody::Draw() {

}

void BossMechBody::DebugDraw() {
	MAGISYSTEM::DrawLineSphere(bodyTrans_->GetWorldPosition(), 0.2f, Color::Red);
}

Transform3D* BossMechBody::GetTransform() {
	return bodyTrans_;
}
