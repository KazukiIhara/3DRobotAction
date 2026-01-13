#pragma once


#include "GameEffects/System/BaseGameEffect/BaseGameEffect.h"

class MechCore;

// 実装メモ
/*
	回避した場所に水色のリングが発生
	EaseOutで広がる
*/

/// <summary>
/// ジャスト回避した際のエフェクト
/// </summary>
class JustDodgeEffect :BaseGameEffect {
public:
	JustDodgeEffect(const Vector3& pos, const Vector3& moveDir);
	~JustDodgeEffect()override = default;

	void Update()override;
	void Draw()override;

};