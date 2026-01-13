#pragma once


#include "GameEffects/System/BaseGameEffect/BaseGameEffect.h"

class MechCore;

/// <summary>
/// ジャスト回避した際のエフェクト
/// </summary>
class JustDodgeEffect :BaseGameEffect {
public:
	using BaseGameEffect::BaseGameEffect;
	~JustDodgeEffect()override = default;

	void Update()override;
	void Draw()override;

};