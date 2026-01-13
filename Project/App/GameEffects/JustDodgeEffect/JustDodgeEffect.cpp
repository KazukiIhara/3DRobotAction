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

	MAGISYSTEM::LoadTexture("gradationToon.png");

	// エフェクトのタイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });

	// 座標をセット
	ringTrans_ = emitPos;
	planeTrans_ = emitPos;

	// エフェクト初期設定

	// リング
	// 形状
	const Vector2 ringDataStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataStart(outer,inner)" });
	ringData_.outerRadius = ringDataStart.x;
	ringData_.innerRadius = ringDataStart.y;
	// マテリアル
	ringMat_.blendMode = BlendMode::Add;
	ringMat_.textureName = "dodgeEffect.png";
	ringMat_.baseColor = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","RingColorStart" });

	// 板ポリ

	// 形状
	planeScale_ = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","JustDodge","PlaneScaleStart" });

	// マテリアル
	planeMat_.blendMode = BlendMode::Add;
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
	// 形状
	const Vector2 ringDataStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataStart(outer,inner)" });
	const Vector2 ringDataEnd = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataEnd(outer,inner)" });
	const Vector2 ringData = SimpleAnimation<Vector2>(ringDataStart, ringDataEnd, EasingType::EaseOutCubic).GetValue(t);
	ringData_.outerRadius = ringData.x;
	ringData_.innerRadius = ringData.y;

	// マテリアル
	const Vector4 ringColorStart = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","RingColorStart" });
	const Vector4 ringColorEnd = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","RingColorEnd" });
	const Vector4 ringColor = SimpleAnimation<Vector4>(ringColorStart, ringColorEnd, EasingType::EaseInCubic).GetValue(t);
	ringMat_.baseColor = ringColor;

	// 板ポリ
	// 形状
	const Vector3 planeScaleStart = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","JustDodge","PlaneScaleStart" });
	const Vector3 planeScaleEnd = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","JustDodge","PlaneScaleEnd" });
	const Vector3 planeScale = SimpleAnimation<Vector3>(planeScaleStart, planeScaleEnd, EasingType::EaseInOutCubic, true, LoopType::PingPong).GetValue(t * 2.0f);
	planeScale_ = planeScale;
}

void JustDodgeEffect::Draw() {

	// カメラからビルボード行列作成
	const Camera3D* currentCamera = MAGISYSTEM::GetCurrentCamera3D();
	const Matrix4x4 ringWMat = currentCamera->MakeBillBoardMat(ringTrans_);
	const Matrix4x4 planeWMat = currentCamera->MakeBillBoardMat(planeTrans_, planeScale_);

	// リング描画
	MAGISYSTEM::DrawRing3D(ringWMat, ringData_, ringMat_);
	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(planeWMat, planeData_, planeMat_);

}
