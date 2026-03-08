#include "LockOnUI.h"

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/ILockOnTarget/ILockOnTarget.h"
#include "Feature/Boss/Mech/BossMech.h"

using namespace MAGIUtility;

LockOnUI::LockOnUI(ILockOnTarget* target) {
	target_ = target;

	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "UI","LockOn","InnerRotSpeed" }, Magi::ParamType::Float);
	MAGISYSTEM::AddParameterData({ "UI","LockOn","BossHPPos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","LockOn","BossHPSize" }, Magi::ParamType::Vec2);

	// テクスチャロード
	MAGISYSTEM::LoadTexture("InnerLock.png");
	MAGISYSTEM::LoadTexture("OuterLock.png");
	MAGISYSTEM::LoadTexture("BossStatusUI.png");
	MAGISYSTEM::LoadTexture("white.png");

	// マテリアルの設定
	innerMat_.textureName = "InnerLock.png";
	innerMat_.anchorPoint = { 0.5f,0.5f };

	outerMat_.textureName = "OuterLock.png";
	outerMat_.anchorPoint = { 0.5f,0.5f };

	bossStatusMat_.textureName = "BossStatusUI.png";
	bossStatusMat_.anchorPoint = { 0.0f,1.0f };

	bossHpMat_.textureName = "white.png";
	bossHpMat_.anchorPoint = { 0.0f,0.5f };
	bossHpMat_.color = Color::Red;

}

void LockOnUI::Update() {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ターゲットのワールド座標を変換
	const Vector3 wPos = target_->GetCenterPos();

	auto screenPos = TransformWorldToScreen(wPos);
	drawFlag_ = screenPos.first;
	screenPos_ = screenPos.second;

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


	// ボスのHPバーの処理
	bossHp_.position = screenPos_ + MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LockOn","BossHPPos" });
	bossHp_.size = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LockOn","BossHPSize" });


	const int32_t hp = dynamic_cast<BossMech*>(target_)->GetStatus()->GetHP();
	const int32_t maxHp = dynamic_cast<BossMech*>(target_)->GetStatus()->GetMaxHP();
	const float hpRate = static_cast<float>(hp) / static_cast<float>(maxHp);

	const float maxLength = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","LockOn","BossHPSize" }).x;
	bossHp_.size.x = maxLength * hpRate;

}

void LockOnUI::Draw() {
	if (drawFlag_) {
		MAGISYSTEM::DrawSprite(inner_, innerMat_);
		MAGISYSTEM::DrawSprite(outer_, outerMat_);
		MAGISYSTEM::DrawSprite(bossStatus_, bossStatusMat_);
		MAGISYSTEM::DrawSprite(bossHp_, bossHpMat_);
	}
}
