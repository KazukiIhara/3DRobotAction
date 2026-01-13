#include "JustDodgeEffect.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"

using namespace Magi;

JustDodgeEffect::JustDodgeEffect(const Vector3& emitPos) :
	BaseGameEffect(emitPos) {

	// エフェクトのタイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });

	// 座標をセット
	ringTrans_ = emitPos;
	planeTrans_ = emitPos;

	// エフェクト初期設定
	
	// リング
	ringMat_.blendMode = BlendMode::Normal;

	// 板ポリ
	planeMat_.blendMode = BlendMode::Normal;

	// パーティクル発生

}

void JustDodgeEffect::Update() {
	// 必要な定数を取得
	const float kEffectTime = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });
	const float kDt = MAGISYSTEM::GetDeltaTime();

}

void JustDodgeEffect::Draw() {
	const Camera3D* currentCamera = MAGISYSTEM::GetCurrentCamera3D();

	const Matrix4x4 ringWMat = currentCamera->MakeBillBoardMat(ringTrans_);
	const Matrix4x4 planeWMat = currentCamera->MakeBillBoardMat(planeTrans_);

	// リング描画
	MAGISYSTEM::DrawRing3D(ringWMat, ringData_, ringMat_);
	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(planeWMat, planeData_, planeMat_);

}
