#include "PlayerUI.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

using namespace MAGIMath;

PlayerUI::PlayerUI() {
	// ロックオン用のスプライト設定
	lockonGrayData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	lockonGrayData_.size = { 256.0f,256.0f };

	lockonGrayMat_.anchorPoint = { 0.5f,0.5f };
	lockonGrayMat_.textureName = "LockonUIGray.png";

	// 射撃盤用のスプライト設定
	lockonRedData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	lockonRedData_.size = { 128.0f,128.0f };

	lockonRedMat_.anchorPoint = { 0.5f,0.5f };
	lockonRedMat_.textureName = "LockonUIRed.png";

	// APバー用のスプライト設定
	apBarData_.position = Vector2(98.0f, 900.0f);

	apBarMaterialData_.anchorPoint = { 0.0f,0.5f };
	apBarMaterialData_.textureName = "ApBar.png";

	// APゲージ用のスプライト設定
	apGaugeData_.position = Vector2(100.0f, 900.0f);

	apGaugeMaterialData_.anchorPoint = { 0.0f,0.5f };
	apGaugeMaterialData_.textureName = "ApGauge.png";

	// ボスAPバー用のスプライト設定
	bossApBarData_.position = Vector2(WindowApp::kClientWidth * 0.5f - kBossAPBarWidth_ * 0.5f, 100.0f);

	bossApBarMaterialData_.anchorPoint = { 0.0f,0.5f };
	bossApBarMaterialData_.textureName = "BossApBar.png";


	// ボスAPゲージ用のスプライト設定
	bossApGaugeData_.position = Vector2(WindowApp::kClientWidth * 0.5f - kBossApGaugeWidth_ * 0.5f, 100.0f);

	bossApGaugeMaterialData_.anchorPoint = { 0.0f,0.5f };
	bossApGaugeMaterialData_.textureName = "BossApGauge.png";

}

void PlayerUI::SetBoss(std::weak_ptr<MechCore> bossMechCore) {
	bossMech_ = bossMechCore;
}

void PlayerUI::Update(MechCore* mechCore) {
	// ロックオンUIの更新
	UpdateLockonUI(mechCore);

	// APUIの更新
	UpdateAPUI(mechCore);

}

void PlayerUI::Draw(MechCore* mechCore) {
	// ロックオンUIの描画
	DrawLockonUI();

	// APUIの描画
	DrawAPUI();

#if defined(DEBUG) || defined(DEVELOP)
	DrawDebugUI(mechCore);
#endif
}


void PlayerUI::UpdateLockonUI(MechCore* mechCore) {
	// ロックオン状態などに応じてUIの座標を更新
	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		if (auto targetBody = target->GetMechBody()->GetGameObject().lock()) {
			// ターゲットのワールド座標を取得
			const Vector3 targetWPos = targetBody->GetTransform()->GetWorldPosition();

			const Matrix4x4 vp = MAGISYSTEM::GetCurrentCamera3D()->GetViewProjectionMatrix();
			Vector4 clip = Transform(Vector4(targetWPos.x, targetWPos.y, targetWPos.z, 1.0f), vp);

			const float invW = 1.0f / clip.w;
			Vector3 ndc{ clip.x * invW, clip.y * invW, clip.z * invW };

			const float sx = (ndc.x + 1.f) * 0.5f * WindowApp::kClientWidth;
			const float sy = (-ndc.y + 1.f) * 0.5f * WindowApp::kClientHeight;

			lockonGrayData_.position = { sx,sy };
			lockonRedData_.position = { sx,sy };
		}
	} else {
		lockonGrayData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
		lockonRedData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	}
}

void PlayerUI::UpdateAPUI(MechCore* mechCore) {
	// 現在のHpを取得
	const int32_t currentHp = mechCore->GetStatusComponent()->GetHp();
	// Hpの最大値を取得
	const int32_t maxHp = mechCore->GetStatusComponent()->GetMaxHp();

	// APゲージの長さを設定
	apGaugeData_.size.x = kAPSpriteWidth_ * (static_cast<float>(currentHp) / static_cast<float>(maxHp));

	// ボスにあたる敵がいる場合
	if (auto bossM = bossMech_.lock()) {
		// 現在のHpを取得
		const int32_t bossCurrentHp = bossM->GetStatusComponent()->GetHp();
		// Hpの最大値を取得
		const int32_t bossMaxHp = bossM->GetStatusComponent()->GetMaxHp();

		// APゲージの長さを設定
		bossApGaugeData_.size.x = (static_cast<float>(bossCurrentHp) / static_cast<float>(bossMaxHp)) * kBossApGaugeWidth_;

	}

}

void PlayerUI::DrawLockonUI() {
	// ロックオンUI
	MAGISYSTEM::DrawSprite(lockonGrayData_, lockonGrayMat_);

	// 射撃盤UI
	MAGISYSTEM::DrawSprite(lockonRedData_, lockonRedMat_);
}

void PlayerUI::DrawAPUI() {
	// APバー
	MAGISYSTEM::DrawSprite(apBarData_, apBarMaterialData_);
	// APゲージ
	MAGISYSTEM::DrawSprite(apGaugeData_, apGaugeMaterialData_);

	// ボスにあたる敵がいる場合
	if (bossMech_.lock()) {
		// ボスのAPバー
		MAGISYSTEM::DrawSprite(bossApBarData_, bossApBarMaterialData_);
		// ボスのゲージ
		MAGISYSTEM::DrawSprite(bossApGaugeData_, bossApGaugeMaterialData_);
	}
}

void PlayerUI::DrawDebugUI(MechCore* mechCore) {
	// 描画したいパラメータ
	Vector3 worldPosition{};
	int hp = 0;
	int targetHP = 0;

	if (auto obj = mechCore->GetGameObject().lock()) {
		worldPosition = obj->GetTransform()->GetWorldPosition();
	}

	hp = mechCore->GetStatusComponent()->GetHp();

	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		targetHP = target->GetStatusComponent()->GetHp();
	}

	ImGui::Begin("PlayerDebugUI");
	ImGui::InputFloat3("WorldPosition", &worldPosition.x);
	ImGui::InputInt("HP", &hp);
	ImGui::InputInt("TargetHP", &targetHP);
	ImGui::End();
}