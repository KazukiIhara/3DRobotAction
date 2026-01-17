#include "BossMechHead.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechHead::BossMechHead(const BossMechHead::InitParam& param, BossMech* mech) {
	// 初期化パラメータを受け取る
	headModelName_ = param.modelName;

	// 機体のポインタを受け取る
	mech_ = mech;
	// トランスフォーム作成
	headTrans_ = MAGISYSTEM::AddTransform3D();

}

void BossMechHead::Update() {

}

void BossMechHead::Draw() {

}

void BossMechHead::DebugDraw() {
	MAGISYSTEM::DrawLineSphere(headTrans_->GetWorldPosition(), 0.2f, Color::Red);
}

Transform3D* BossMechHead::GetHeadTransform() {
	return headTrans_;
}

