#pragma once

#include <memory>
#include <array>

#include "Feature/Effect/System/BaseGameEffect/BaseGameEffect.h"
#include "Structs/SpriteStruct.h"
#include "Math/Types/AllMathTypes.h"

class BossMech;

// 実装メモ

/*
	ボスの目の部分に十字のスプライトを出す
*/

/// <summary>
/// ボス攻撃前エフェクト
/// </summary>
class BossAttackWarningEffect:public BaseGameEffect {
public:
	BossAttackWarningEffect(const Vector3& emitPos, BossMech* mech);
	~BossAttackWarningEffect()override = default;

	void Update()override;
	void Draw()override;

private:
	float timer_ = 0.0f;

	std::array<SpriteData, 2> sprite_;
	SpriteMaterialData mat_;

	BossMech* mech_ = nullptr;
};