#include "PilotScoreUI.h"

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/Pilot/Mech/PilotMech.h"

using namespace MAGIUtility;

PilotScoreUI::PilotScoreUI(PilotMech* pilotMech) {
	mech_ = pilotMech;

	// パラメータの追加
	MAGISYSTEM::AddParameterData({ "UI","PilotScore","Text","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","PilotScore","Bar","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","PilotScore","Gauge","Pos" }, Magi::ParamType::Vec2);
	MAGISYSTEM::AddParameterData({ "UI","PilotScore","Gauge","Size" }, Magi::ParamType::Vec2);


	MAGISYSTEM::AddParameterData({ "UI","PilotScore","AttackMul","Pos" }, Magi::ParamType::Vec2);


	// テクスチャのロード
	MAGISYSTEM::LoadTexture("Default.png");
	MAGISYSTEM::LoadTexture("Cool.png");
	MAGISYSTEM::LoadTexture("Better.png");
	MAGISYSTEM::LoadTexture("Amazing!.png");
	MAGISYSTEM::LoadTexture("Stylish!!.png");

	MAGISYSTEM::LoadTexture("PilotScoreBar.png");
	MAGISYSTEM::LoadTexture("White.png");


	// UI座標を設定
	text_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Text","Pos" });
	bar_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Bar","Pos" });
	gauge_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Gauge","Pos" });
	attackMulPos_ = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","AttackMul","Pos" });

	// Gaugeサイズ設定
	gauge_.size = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Gauge","Size" });

	// マテリアルの設定
	textMat_.textureName = "Default.png";
	barMat_.textureName = "PilotScoreBar.png";
	gaugeMat_.textureName = "white.png";
	gaugeMat_.anchorPoint = { 0.0f,0.5f };
}

void PilotScoreUI::Update() {
	// UI座標を更新
	text_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Text","Pos" });
	bar_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Bar","Pos" });
	gauge_.position = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Gauge","Pos" });

	attackMulPos_ = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","AttackMul","Pos" });

	// Gaugeサイズ設定
	gauge_.size = MAGISYSTEM::GetParameterValue<Vector2>({ "UI","PilotScore","Gauge","Size" });

	const float gauge = mech_->GetStatus()->GetJustDodgeGauge();
	gauge_.size.x *= gauge;

	// テキスト変更
	const int32_t score = mech_->GetStatus()->GetJustDodgeScore();
	switch (score) {
	case 0:
		textMat_.textureName = "Default.png";
		break;
	case 1:
		textMat_.textureName = "Cool.png";
		break;
	case 2:
		textMat_.textureName = "Better.png";
		break;
	case 3:
		textMat_.textureName = "Amazing!.png";
		break;
	case 4:
		textMat_.textureName = "Stylish!!.png";
		break;
	default:
		textMat_.textureName = "Stylish!!.png";
		break;
	}

}

void PilotScoreUI::Draw() {
	const float attackMul = mech_->GetStatus()->GetAttackMul();
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << attackMul;
	const std::string s = oss.str();

	MAGISYSTEM::DrawFont("Damage x" + s, attackMulPos_, Color::White, 0.2f);
	MAGISYSTEM::DrawSprite(text_, textMat_);
	MAGISYSTEM::DrawSprite(bar_, barMat_);
	MAGISYSTEM::DrawSprite(gauge_, gaugeMat_);
}