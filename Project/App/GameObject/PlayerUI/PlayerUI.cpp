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

	lockonRedData_.position = Vector2(WindowApp::kClientWidth * 0.5f, WindowApp::kClientHeight * 0.5f);
	lockonRedData_.size = { 128.0f,128.0f };

	lockonRedMat_.anchorPoint = { 0.5f,0.5f };
	lockonRedMat_.textureName = "LockonUIRed.png";
	lockonRedMat_.blendmode = BlendMode::Normal;
}

void PlayerUI::Update(MechCore* mechCore) {
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

void PlayerUI::Draw(MechCore* mechCore) {
	// ロックオンUIの描画
	DrawLockonUI();

#if defined(DEBUG) || defined(DEVELOP)
	DrawDebugUI(mechCore);
#endif
}


void PlayerUI::DrawLockonUI() {

	MAGISYSTEM::DrawSprite(lockonGrayData_, lockonGrayMat_);

	MAGISYSTEM::DrawSprite(lockonRedData_, lockonRedMat_);
}

void PlayerUI::DrawDebugUI(MechCore* mechCore) {
	// 描画したいパラメータ
	Vector3 worldPosition{};
	int hp = mechCore->GetStatusComponent()->GetHp();


	if (auto obj = mechCore->GetGameObject().lock()) {
		worldPosition = obj->GetTransform()->GetWorldPosition();
	}

	ImGui::Begin("PlayerDebugUI");
	ImGui::InputFloat3("WorldPosition", &worldPosition.x);
	ImGui::InputInt("HP", &hp);
	ImGui::End();
}