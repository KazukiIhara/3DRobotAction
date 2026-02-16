#include "LockOnUI.h"

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/ILockOnTarget/ILockOnTarget.h"

using namespace MAGIUtility;

LockOnUI::LockOnUI(ILockOnTarget* target) {
	target_ = target;

	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "UI","LockOn","InnerRotSpeed" }, Magi::ParamType::Float);

	// テクスチャロード
	MAGISYSTEM::LoadTexture("InnerLock.png");
	MAGISYSTEM::LoadTexture("OuterLock.png");
	MAGISYSTEM::LoadTexture("BossStatusUI.png");

	// マテリアルの設定
	innerMat_.textureName = "InnerLock.png";
	innerMat_.anchorPoint = { 0.5f,0.5f };

	outerMat_.textureName = "OuterLock.png";
	outerMat_.anchorPoint = { 0.5f,0.5f };

	bossStatusMat_.textureName = "BossStatusUI.png";
	bossStatusMat_.anchorPoint = { 0.0f,1.0f };

}

void LockOnUI::Update() {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ターゲットのワールド座標を変換
	const Vector3 wPos = target_->GetCenterPos();
	screenPos_ = TransformWorldToScreen(wPos);

	// インナーUIを回転
	const float innerRotSpeed = MAGISYSTEM::GetParameterValue<float>({ "UI","LockOn","InnerRotSpeed" });
	innerRot_ += innerRotSpeed * dt;
	if (innerRot_ >= std::numbers::pi_v<float>*2.0f) {
		innerRot_ = 0.0f;
	}
	inner_.rotate = innerRot_;

	// UIの座標を設定
	inner_.position = screenPos_;
	outer_.position = screenPos_;
	bossStatus_.position = screenPos_;
}

void LockOnUI::Draw() {
	MAGISYSTEM::DrawSprite(inner_, innerMat_);
	MAGISYSTEM::DrawSprite(outer_, outerMat_);
	MAGISYSTEM::DrawSprite(bossStatus_, bossStatusMat_);
}
