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
	lockonGrayMat_.blendmode = BlendMode::Normal;

	// 射撃盤用のスプライト設定
	lockonRedData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	lockonRedData_.size = { 128.0f,128.0f };

	lockonRedMat_.anchorPoint = { 0.5f,0.5f };
	lockonRedMat_.textureName = "LockonUIRed.png";
	lockonRedMat_.blendmode = BlendMode::Normal;

	// APゲージ用のスプライト設定
	apGaugeData_.position = Vector2(100.0f, 900.0f);
	apGaugeData_.size = { kAPSpriteWidth_,16.0f };

	apGaugeMaterialData_.anchorPoint = { 0.0f,0.5f };
	apGaugeMaterialData_.textureName = "ApGauge.png";

	// APバー用のスプライト設定


	// APUI用のスプライト設定

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
	apGaugeData_.size.x = currentHp;

}

void PlayerUI::DrawLockonUI() {
	// ロックオンUI
	MAGISYSTEM::DrawSprite(lockonGrayData_, lockonGrayMat_);

	// 射撃盤UI
	MAGISYSTEM::DrawSprite(lockonRedData_, lockonRedMat_);
}

void PlayerUI::DrawAPUI() {


	// APゲージ
	MAGISYSTEM::DrawSprite(apGaugeData_, apGaugeMaterialData_);
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