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

}

void PlayerUI::Draw() {
	DrawLockonUI();
}

void PlayerUI::DrawLockonUI() {

	MAGISYSTEM::DrawSprite(lockonGrayData_, lockonGrayMat_);
	MAGISYSTEM::DrawSprite(lockonRedData_, lockonRedMat_);
}