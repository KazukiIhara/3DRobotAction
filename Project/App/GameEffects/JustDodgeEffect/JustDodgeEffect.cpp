#define NOMINMAX

#include "JustDodgeEffect.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"

using namespace Magi;

JustDodgeEffect::JustDodgeEffect(const Vector3& emitPos) :
	BaseGameEffect(emitPos) {

	// エディットシーン用に一応ロード処理を書く
	MAGISYSTEM::LoadTexture("gradation.png");
	MAGISYSTEM::LoadTexture("dodgeEffect.png");

	// エフェクトのタイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });

	// 座標をセット
	ringTrans_ = emitPos;
	planeTrans_ = emitPos;

	// エフェクト初期設定
	// リング
	ringMat_.blendMode = BlendMode::Normal;
	ringMat_.textureName = "gradation.png";

	// リング開始時形状
	const Vector2 ringDataStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataStart(outer,inner)" });
	ringData_.outerRadius = ringDataStart.x;
	ringData_.innerRadius = ringDataStart.y;


	// 板ポリ
	planeMat_.blendMode = BlendMode::Normal;
	planeMat_.textureName = "dodgeEffect.png";

	// パーティクル発生

}

void JustDodgeEffect::Update() {
	// デルタタイムを取得
	const float kDt = MAGISYSTEM::GetDeltaTime();
	// タイマーを更新
	time_ -= kDt;
	time_ = std::max(0.0f, time_);

	// タイマーが0になったら生存フラグを切る
	if (time_ == 0.0f) {
		isAlive_ = false;
		return;
	}

	// 定数取得
	const float kEffectTime = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });
	const float t = 1.0f - time_ / kEffectTime;

	// 形状データをあれこれする

	// リング
	const Vector2 ringDataStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataStart(outer,inner)" });
	const Vector2 ringDataEnd = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataEnd(outer,inner)" });
	const Vector2 ringData = SimpleAnimation<Vector2>(ringDataStart, ringDataEnd).GetValue(t);

	// リングの形状を更新
	ringData_.outerRadius = ringData.x;
	ringData_.innerRadius = ringData.y;

	// リングのカラーを更新

}

void JustDodgeEffect::Draw() {

	// カメラからビルボード行列作成
	const Camera3D* currentCamera = MAGISYSTEM::GetCurrentCamera3D();
	const Matrix4x4 ringWMat = currentCamera->MakeBillBoardMat(ringTrans_);
	const Matrix4x4 planeWMat = currentCamera->MakeBillBoardMat(planeTrans_);

	// リング描画
	MAGISYSTEM::DrawRing3D(ringWMat, ringData_, ringMat_);
	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(planeWMat, planeData_, planeMat_);

}
