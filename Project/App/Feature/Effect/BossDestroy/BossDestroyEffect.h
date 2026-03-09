#pragma once

#include <memory>

#include "Feature/Effect/System/BaseGameEffect/BaseGameEffect.h"
#include "Structs/Primitive3DStruct.h"
#include "Math/Types/AllMathTypes.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// ボス撃破時のエフェクト
/// 複数の爆発や火花エフェクトを発生させるクラス
/// </summary>
class BossDestroyEffect :public BaseGameEffect {
public:
	BossDestroyEffect(const Vector3& emitPos, GameEffectManager* gameEffectManager);
	~BossDestroyEffect()override = default;

	void Update()override;
	void Draw()override;

private:
	// エフェクトマネージャの参照ポインタ
	GameEffectManager* gameEffectManager_ = nullptr;

	// エフェクトタイマー
	float time_ = 0.0f;

	// 次の爆発までのタイマー
	float explosionTimer_ = 0.0f;
};