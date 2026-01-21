#pragma once

#include <memory>

#include "Feature/Effect/System/BaseGameEffect/BaseGameEffect.h"
#include "Structs/Primitive3DStruct.h"
#include "Math/Types/AllMathTypes.h"

// 実装メモ
/*
	回避した場所に水色の細いリングが発生、	EaseOutで広がる
	回避した場所に横長の板ポリが発生、徐々に細くなって消える
	回避した場所にパーティクルが発生、リングと同じように拡散する

	必要なもの
	・リング
	・板ポリ
	・発散パーティクル
*/

/// <summary>
/// ジャスト回避した際のエフェクト
/// </summary>
class JustDodgeEffect:public BaseGameEffect {
public:
	JustDodgeEffect(const Vector3& emitPos);
	~JustDodgeEffect()override = default;

	void Update()override;
	void Draw()override;

private:
	// エフェクトタイマー
	float time_ = 0.0f;

	// リングのエフェクト用変数
	Vector3	ringTrans_;
	RingData3D ringData_;
	MaterialData3D ringMat_;

	// 板ポリエフェクト用変数
	Vector3	planeTrans_;
	Vector3 planeScale_;
	PlaneData3D planeData_;
	MaterialData3D planeMat_;
};