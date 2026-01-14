#pragma once

#include "Math/Types/AllMathTypes.h"

// 実装メモ
/*
	エフェクトとは書いてあるが、実際にはワールド座標と生存フラグを持っているだけのもの
	これを基底クラスとして作ったエフェクトを、シーン内のGameEffectManagerに突っ込こむことで更新と描画を自動で行う
	エフェクトが終わったら更新処理内で生存フラグをFalseにすること
*/

/// <summary>
/// ゲームエフェクト基底クラス
/// </summary>
class BaseGameEffect {
public:
	BaseGameEffect(const Vector3& emitPos);
	virtual ~BaseGameEffect() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	bool IsAlive()const;

protected:
	Vector3 worldPos_{};
	bool isAlive_ = true;
};