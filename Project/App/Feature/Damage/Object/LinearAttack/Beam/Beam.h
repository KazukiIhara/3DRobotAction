#pragma once

// 基底クラス
#include "Feature/Damage/Object/LinearAttack/Base/BaseLinearAttack.h"

// エフェクトマネージャ
class GameEffectManager;

// 実装メモ
/*
	地形に当たったら爆発とかやりたくない？俺はやりたい
*/

/// <summary>
/// ビーム
/// </summary>
class Beam :public BaseLinearAttack {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseLinearAttack::InitParam baseInit;
		float thickness = 0.0f;
	};
public:
	Beam(const Beam::InitParam& initParam, RefContext ref);
	~Beam()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;
private:

	float thickness_;

	GameEffectManager* effectManager_;
};