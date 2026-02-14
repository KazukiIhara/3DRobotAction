#include "LockOnUI.h"

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/ILockOnTarget/ILockOnTarget.h"

using namespace MAGIUtility;

LockOnUI::LockOnUI(ILockOnTarget* target) {
	target_ = target;

	// テクスチャロード
	MAGISYSTEM::LoadTexture("InnerLock.png");
	MAGISYSTEM::LoadTexture("OuterLock.png");

	// マテリアルの設定
	innerMat_.textureName = "InnerLock.png";
	innerMat_.anchorPoint = { 0.5f,0.5f };

	outerMat_.textureName = "OuterLock.png";
	outerMat_.anchorPoint = { 0.5f,0.5f };

}

void LockOnUI::Update() {
	// ターゲットのワールド座標を変換
	const Vector3 wPos = target_->GetCenterPos();
	screenPos_ = TransformWorldToScreen(wPos);

	// UIの座標を設定
	inner_.position = screenPos_;
	outer_.position = screenPos_;

}

void LockOnUI::Draw() {
	MAGISYSTEM::DrawSprite(inner_, innerMat_);
	MAGISYSTEM::DrawSprite(outer_, outerMat_);
}
