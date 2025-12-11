#include "PlayerUI.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

using namespace magi;
using namespace MAGIMath;
using namespace MAGIUtility;

PlayerUI::PlayerUI() {
	// ロックオン用のスプライト設定
	lockonGrayData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	lockonGrayData_.size = { 256.0f,256.0f };

	lockonGrayMat_.anchorPoint = { 0.5f,0.5f };
	lockonGrayMat_.textureName = "LockonUIGray.png";

	// 射撃盤用のスプライト設定
	lockonRedData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	lockonRedData_.size = { 192.0f,192.0f };
	lockonRedMat_.anchorPoint = { 0.5f,0.5f };
	lockonRedMat_.textureName = "LockonUIRed.png";

	// APバー用のスプライト設定
	apBarData_.position = Vector2(98.0f, 250.0f);
	apBarMaterialData_.anchorPoint = { 0.0f,0.5f };
	apBarMaterialData_.textureName = "ApBar.png";

	// APゲージ用のスプライト設定
	apGaugeData_.position = Vector2(100.0f, 250.0f);
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

	// エネルギーUI用のスプライト設定
	energyBarData_.position = energyBarPos_;
	energyBarMatData_.anchorPoint = { 0.5f,1.0f };
	energyBarMatData_.textureName = "EnergyUI_Bar.png";

	// エネルギーバー用のスプライト設定
	energyGaugeData_.position = energyGaugePos_;
	energyGaugeMatData_.anchorPoint = { 0.5f,1.0f };
	energyGaugeMatData_.textureName = "EnergyUI_Gauge.png";
	energyGaugeMatData_.color = Color::Green;

	// エネルギーテキスト用のスプライト設定
	energyTextData_.position = energyTextPos_;
	energyTextMatData_.textureName = "EnergyUI.png";


	// 右手武器情報スプライト
	rightHandWeaponData_.position = rightHandWeaponPos_;



	// 左手武器情報スプライト
	leftHandWeaponData_.position = leftHandWeaponPos_;


}

void PlayerUI::SetBoss(std::weak_ptr<MechCore> bossMechCore) {
	bossMech_ = bossMechCore;
}

void PlayerUI::Update(MechCore* mechCore) {
	// ロックオンUIの更新
	UpdateLockonUI(mechCore);

	// APUIの更新
	UpdateAPUI(mechCore);

	// ENUIの更新
	UpdateENUI(mechCore);
}

void PlayerUI::Draw([[maybe_unused]] MechCore* mechCore) {
	// ロックオンUIの描画
	DrawLockonUI();

	// APUIの描画
	DrawAPUI();

	// ENUIの描画
	DrawENUI();

#if defined(DEBUG) || defined(DEVELOP)
	DrawDebugUI(mechCore);
#endif
}

void PlayerUI::UpdateLockonUI(MechCore* mechCore) {

	// ロックオン状態などに応じてUIの座標を更新
	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		if (auto targetBody = target->GetMechBody()->GetGameObject().lock()) {
			// ロックオン係数
			const float lockonFactor = mechCore->GetStatusComponent()->GetFcsAvoidFactor();

			// FCSが外れたフレームに座標を記録
			if (lockonFactor == 0.05f) {
				lerpStartPos_ = lockOnWPos_;
			}

			// ターゲットのワールド座標を取得
			// ロックオン対象の胴体のワールド座標を取得
			const Vector3 targetBodyPos = targetBody->GetTransform()->GetWorldPosition();
			const Vector2 targetSPos = TransformWorldToScreen(targetBodyPos);

			// 対象に補間移動
			lockOnWPos_ = Lerp(lerpStartPos_, targetBodyPos, lockonFactor);
			const Vector2 targetRedSPos = TransformWorldToScreen(lockOnWPos_);

			// 座標設定
			lockonGrayData_.position = targetSPos;
			lockonRedData_.position = targetRedSPos;

			// 色変更処理
			if (lockonFactor == 1.0f) {
				lockonRedMat_.color = Color::Green;
			} else {
				lockonRedMat_.color = Color::Red;
			}
		}

	} else {
		// 非ロックオン時の照準位置の初期化
		if (auto body = mechCore->GetMechBody()->GetGameObject().lock()) {
			lockOnWPos_ = body->GetTransform()->GetWorldPosition();
		}
		// 中心座標
		lockonGrayData_.position = Vector2(magi::WindowApp::kClientWidth * 0.5f, magi::WindowApp::kClientHeight * 0.5f);
		lockonRedData_.position = Vector2(magi::WindowApp::kClientWidth * 0.5f, magi::WindowApp::kClientHeight * 0.5f);

		// 非ロックオン時は常に赤色
		lockonRedMat_.color = Color::Red;
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

void PlayerUI::UpdateENUI(MechCore* mechCore) {
	// ポジションセット
	energyBarData_.position = energyBarPos_;
	energyGaugeData_.position = energyGaugePos_;
	energyTextData_.position = energyTextPos_;

	// 現在のenを取得
	const int32_t currentEn = mechCore->GetStatusComponent()->GetEn();
	// enの最大値を取得
	const int32_t maxEn = mechCore->GetStatusComponent()->GetMaxEn();

	// オーバーヒートかどうかを取得
	const bool isOverHeat = mechCore->GetStatusComponent()->GetIsOverheat();

	// オーバーヒートなら色を変える
	if (isOverHeat) {
		energyGaugeMatData_.color = Color::Red;
	} else {
		energyGaugeMatData_.color = Color::Green;
	}

	// ENゲージの長さを設定
	energyGaugeData_.size.y = kEnergyGaugeHeight_ * (static_cast<float>(currentEn) / static_cast<float>(maxEn));

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

void PlayerUI::DrawENUI() {

	// エネルギーゲージ
	MAGISYSTEM::DrawSprite(energyBarData_, energyBarMatData_);
	MAGISYSTEM::DrawSprite(energyGaugeData_, energyGaugeMatData_);
	MAGISYSTEM::DrawSprite(energyTextData_, energyTextMatData_);

}

void PlayerUI::DrawWeaponUI() {


}

void PlayerUI::DrawDebugUI(MechCore* mechCore) {
	// 描画したいパラメータ
	Vector3 worldPosition{};
	int hp = 0;
	int en = 0;
	int rightHandAmmo = 0;
	int leftHandAmmo = 0;
	int targetHP = 0;
	Vector3 targetPos{};

	if (auto obj = mechCore->GetGameObject().lock()) {
		worldPosition = obj->GetTransform()->GetWorldPosition();
	}

	hp = mechCore->GetStatusComponent()->GetHp();
	en = mechCore->GetStatusComponent()->GetEn();

	rightHandAmmo = mechCore->GetRightHandWeapon()->GetData().ammo_;
	leftHandAmmo = mechCore->GetLeftHandWeapon()->GetData().ammo_;

	if (auto target = mechCore->GetLockOnComponent()->GetLockOnTarget().lock()) {
		targetHP = target->GetStatusComponent()->GetHp();
		// ターゲット対象の座標を取得
		targetPos = target->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();
	}

	// 距離を図る
	float distance = Length(worldPosition - targetPos);

	ImGui::Begin("PlayerDebugUI");
	ImGui::InputFloat3("WorldPosition", &worldPosition.x);
	ImGui::InputInt("HP", &hp);
	ImGui::InputInt("EN", &en);
	ImGui::InputInt("RightHandAmmo", &rightHandAmmo);
	ImGui::InputInt("LeftHandAmmo", &leftHandAmmo);
	ImGui::InputInt("TargetHP", &targetHP);
	ImGui::InputFloat("TargetDistance", &distance);

	ImGui::DragFloat2("EnegryUIPos", &energyBarPos_.x, 1.0f);
	ImGui::DragFloat2("EnergyUITextPos", &energyTextPos_.x, 1.0f);
	ImGui::DragFloat2("EnegryUIGaugePos", &energyGaugePos_.x, 1.0f);

	ImGui::End();
}